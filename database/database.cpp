#include "database.h"

// Qt headers
#include <QFile>
#include <QMessageBox>

// Database headers
#include "city.h"
#include "club.h"
#include "club_comp.h"
#include "club_comp_history.h"
#include "colour.h"
#include "continent.h"
#include "name.h"
#include "nation.h"
#include "non_player.h"
#include "official.h"
#include "player.h"
#include "stadium.h"
#include "staff.h"
#include "staff_comp.h"
#include "staff_comp_history.h"
#include "staff_history.h"
#include "staff_preferences.h"


/* ================== */
/*      Database      */
/* ================== */

// --- Default constructor --- //
Database::Database()
{
}

// --- Destructor --- //
Database::~Database()
{
}


/* ========================= */
/*      Data Validation      */
/* ========================= */

// --- Validate all of the data in the database --- //
void Database::validate(ProgressWindow *p)
{
    bool newProgressWindow;
    const int validationCount = 8;

    if(p == nullptr) {
        p = new ProgressWindow("Validating database", validationCount);
        newProgressWindow = true;
    }
    else {
        p->increaseRange(validationCount);
        newProgressWindow = false;
    }

    // Reset/clear club rosters
    p->increment("Resetting club rosters");
    Club::clearRosters();

    // Reset name counters
    p->increment("Resetting name counters");
    CommonName::resetCounters();
    FirstName::resetCounters();
    SecondName::resetCounters();

    // Set name string text
    p->increment("Setting name string data");
    CommonName::setStringText();
    FirstName::setStringText();
    SecondName::setStringText();

    // Reset nation counters
    p->increment("Resetting nation counters");
    Nation::resetCounters();

    // Clubs (including nation counters)
    p->increment("Validating club data");
    Club::validate();

    // Officials (including name counters)
    p->increment("Validating official data");
    Official::validate();

    // Staff (including name & nation counters)
    p->increment("Validating staff data");
    Staff::validate();

    // Index
    p->increment("Updating index.dat");
    // Offset tracker
    int offset = 0;

    // Set table sizes
    Index::db[Index::CLUB_TABLE].set(Club::dbDom.size());
    Index::db[Index::NATION_CLUBS_TABLE].set(Club::dbInt.size());
    Index::db[Index::COLOUR_TABLE].set(Colour::db.size());
    Index::db[Index::CONTINENT_TABLE].set(Continent::db.size());
    Index::db[Index::NATION_TABLE].set(Nation::db.size());
    Index::db[Index::OFFICIALS_TABLE].set(Official::db.size());
    Index::db[Index::STADIUM_TABLE].set(Stadium::db.size());
    Index::db[Index::STAFF_TABLE].set(Staff::db.size());
    offset += (Index::db[Index::STAFF_TABLE].getRecordCount() * DB_STAFF_SIZE);
    Index::db[Index::NON_PLAYER_TABLE].set(NonPlayer::db.size(), offset);
    offset += (Index::db[Index::NON_PLAYER_TABLE].getRecordCount() * DB_NON_PLAYER_SIZE);
    Index::db[Index::PLAYER_TABLE].set(Player::db.size(), offset);
    Index::db[Index::STAFF_COMP_TABLE].set(StaffComp::db.size());
    Index::db[Index::CITY_TABLE].set(City::db.size());
    Index::db[Index::CLUB_COMP_TABLE].set(ClubComp::dbDom.size());
    Index::db[Index::NATION_COMP_TABLE].set(ClubComp::dbInt.size());
    Index::db[Index::FIRST_NAME_TABLE].set(FirstName::db.size());
    Index::db[Index::SECOND_NAME_TABLE].set(SecondName::db.size());
    Index::db[Index::COMMON_NAME_TABLE].set(CommonName::db.size());
    Index::db[Index::STAFF_HISTORY_TABLE].set(StaffHistory::db.size());
    Index::db[Index::STAFF_COMP_HISTORY_TABLE].set(StaffCompHistory::db.size());
    Index::db[Index::CLUB_COMP_HISTORY_TABLE].set(ClubCompHistory::dbDom.size());
    Index::db[Index::NATION_COMP_HISTORY_TABLE].set(ClubCompHistory::dbInt.size());
    offset += (Index::db[Index::PLAYER_TABLE].getRecordCount() * DB_PLAYER_SIZE);
    Index::db[Index::STAFF_PREFERENCES_TABLE].set(StaffPreferences::db.size(), offset);

    // Clean up progress bar
    if(newProgressWindow) {
        p->complete();
        delete p;
    }
}

/* ============================ */
/*      File I/O: Dat File      */
/* ============================ */

// --- Read a dat file --- //
template<typename T> bool Database::read(QString filename,
                                         QVector<T> &data,
                                         int recordSize,
                                         int skipBytes,
                                         int recordCount)
{
    m_Progress->increment(filename);

    QFile f(m_Folder + filename);

    if(!f.open(QIODevice::ReadOnly)) {
        m_FileErrorList << filename;
        return false;
    }

    else {
        data.clear();

        f.seek(skipBytes);

        // If the record count has not been passed to the function, calculate it using the file size divided by record size.
        // This prevents the issue where a file (e.g. nation_comp_history.dat has an extra byte at the end of the file which
        // is then read as a garbage record (and will then be saved as a garbage record). Such extraneous bytes will be ignored
        // because the record count is rounded down to the nearest whole number when cast to an integer per the below calculation.
        if(recordCount < 0)
            recordCount = static_cast<int>(f.size() / recordSize);

        // Read the defined number of records
        for(int i = 0; i < recordCount; ++i) {
            T buffer;
            f.read((char*)&buffer, recordSize);
            data.push_back(buffer);
        }

        f.close();
        return true;
    }
}

// --- Read an index.dat file --- //
bool Database::readIndex(QString filename, QVector<Index> &data, int recordSize)
{
    m_Progress->increment(filename);

    QFile f(m_Folder + filename);

    if(!f.open(QIODevice::ReadOnly)) {
        m_FileErrorList << filename;
        return false;
    }

    else {
        data.clear();

        // Read header
        f.read((char*)&Index::s_Header, Index::INDEX_HEADER_SIZE);

        // Read records
        while(!f.atEnd()) {
            Index buffer;
            f.read((char*)&buffer, recordSize);
            data.push_back(buffer);
        }

        f.close();
        return true;
    }
}

// --- Write a dat file --- //
template<typename T> bool Database::write(QString filename,
                                          QVector<T> &data,
                                          int recordSize,
                                          bool append)
{    
    m_Progress->increment(filename);

    QFile f(m_Folder + filename);

    // Truncate if the append bool is false
    if(append == false && !f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        m_FileErrorList << filename;
        return false;
    }

    // Append if the append bool is true
    else if(append == true && !f.open(QIODevice::WriteOnly | QIODevice::Append)) {
        m_FileErrorList << filename;
        return false;
    }

    // Write the data
    else {
        for(T itr : data)
            f.write((char*)&itr, recordSize);

        f.close();
        return true;
    }
}

// --- Write an index.dat file --- //
bool Database::writeIndex(QString filename, QVector<Index> &data, int recordSize)
{
    m_Progress->increment(filename);

    QFile f(m_Folder + filename);

    if(!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        m_FileErrorList << filename;
        return false;
    }

    else {
        // Write header
        f.write((char*)&Index::s_Header, Index::INDEX_HEADER_SIZE);

        // Write records
        for(Index itr : data)
            f.write((char*)&itr, recordSize);

        f.close();
        return true;
    }
}


/* ======================== */
/*      Error Checking      */
/* ======================== */

// --- Check for errors --- //
bool Database::errorCheck()
{
    if(m_FileErrorList.size() <= 0)
        return true;

    QString message("The following file(s) could not be accessed:\n");

    for(QString str : m_FileErrorList)
        message.append("\n" + str);

    QMessageBox m;
    m.setWindowTitle("File Error");
    m.setText(message);
    m.setModal(true);
    m.exec();

    return false;
}


/* ============================ */
/*      File I/O: Database      */
/* ============================ */

// --- Load the database --- //
bool Database::load(const QString &folder)
{
    // Clear error list and set database folder path
    m_FileErrorList.clear();
    m_Folder = folder;
    m_Folder.remove("index.dat");   // Remove any reference to index.dat if this was included in the folder string

    // Progress dialog
    m_Progress = new ProgressWindow();
    m_Progress->set("Loading Database", FILE_COUNT);

    // Read the data
    this->readIndex("index.dat", Index::db, DB_INDEX_SIZE);
    this->read("city.dat", City::db, DB_CITY_SIZE);
    this->read("club.dat", Club::dbDom, DB_CLUB_SIZE);
    this->read("club_comp.dat", ClubComp::dbDom, DB_CLUB_COMP_SIZE);
    this->read("club_comp_history.dat", ClubCompHistory::dbDom, DB_CLUB_COMP_HISTORY_SIZE);
    this->read("colour.dat", Colour::db, DB_COLOUR_SIZE);
    this->read("common_names.dat", CommonName::db, DB_NAME_SIZE);
    this->read("continent.dat", Continent::db, DB_CONTINENT_SIZE);
    this->read("first_names.dat", FirstName::db, DB_NAME_SIZE);
    this->read("nat_club.dat", Club::dbInt, DB_CLUB_SIZE);
    this->read("nation.dat", Nation::db, DB_NATION_SIZE);
    this->read("nation_comp.dat", ClubComp::dbInt, DB_CLUB_COMP_SIZE);
    this->read("nation_comp_history.dat", ClubCompHistory::dbInt, DB_CLUB_COMP_HISTORY_SIZE);
    this->read("officials.dat", Official::db, DB_OFFICIAL_SIZE);
    this->read("second_names.dat", SecondName::db, DB_NAME_SIZE);
    this->read("stadium.dat", Stadium::db, DB_STADIUM_SIZE);
    this->read("staff.dat", Staff::db, DB_STAFF_SIZE,
               Index::db[Index::STAFF_TABLE].getOffset(), Index::db[Index::STAFF_TABLE].getRecordCount());
    this->read("staff.dat", NonPlayer::db, DB_NON_PLAYER_SIZE,
               Index::db[Index::NON_PLAYER_TABLE].getOffset(), Index::db[Index::NON_PLAYER_TABLE].getRecordCount());
    this->read("staff.dat", Player::db, DB_PLAYER_SIZE,
               Index::db[Index::PLAYER_TABLE].getOffset(), Index::db[Index::PLAYER_TABLE].getRecordCount());
    this->read("staff.dat", StaffPreferences::db, DB_STAFF_PREFERENCES_SIZE,
               Index::db[Index::STAFF_PREFERENCES_TABLE].getOffset(), Index::db[Index::STAFF_PREFERENCES_TABLE].getRecordCount());
    this->read("staff_comp.dat", StaffComp::db, DB_STAFF_COMP_SIZE);
    this->read("staff_comp_history.dat", StaffCompHistory::db, DB_STAFF_COMP_HISTORY_SIZE);
    this->read("staff_history.dat", StaffHistory::db, DB_STAFF_HISTORY_SIZE);

    // Validate the database data
    this->validate(m_Progress);

    m_Progress->complete();
    delete m_Progress;
    return this->errorCheck();
}

// --- Save the database --- //
bool Database::save(const QString &folder)
{
    // Clear error list and set database folder path
    m_FileErrorList.clear();
    m_Folder = folder;
    m_Folder.remove("index.dat");   // Remove any reference to index.dat if this was included in the folder string

    // Progress dialog
    m_Progress = new ProgressWindow();
    m_Progress->set("Loading Database", FILE_COUNT);

    // Validate the database data
    this->validate(m_Progress);

    // Write the data
    this->writeIndex("index.dat", Index::db, DB_INDEX_SIZE);
    this->write("city.dat", City::db, DB_CITY_SIZE);
    this->write("club.dat", Club::dbDom, DB_CLUB_SIZE);
    this->write("club_comp.dat", ClubComp::dbDom, DB_CLUB_COMP_SIZE);
    this->write("club_comp_history.dat", ClubCompHistory::dbDom, DB_CLUB_COMP_HISTORY_SIZE);
    this->write("colour.dat", Colour::db, DB_COLOUR_SIZE);
    this->write("common_names.dat", CommonName::db, DB_NAME_SIZE);
    this->write("continent.dat", Continent::db, DB_CONTINENT_SIZE);
    this->write("first_names.dat", FirstName::db, DB_NAME_SIZE);
    this->write("nat_club.dat", Club::dbInt, DB_CLUB_SIZE);
    this->write("nation.dat", Nation::db, DB_NATION_SIZE);
    this->write("nation_comp.dat", ClubComp::dbInt, DB_CLUB_COMP_SIZE);
    this->write("nation_comp_history.dat", ClubCompHistory::dbInt, DB_CLUB_COMP_HISTORY_SIZE);
    this->write("officials.dat", Official::db, DB_OFFICIAL_SIZE);
    this->write("second_names.dat", SecondName::db, DB_NAME_SIZE);
    this->write("stadium.dat", Stadium::db, DB_STADIUM_SIZE);
    // Staff.dat tables must be written in the correct order:
    this->write("staff.dat", Staff::db, DB_STAFF_SIZE, false);                          // 1) Staff (truncate)
    this->write("staff.dat", NonPlayer::db, DB_NON_PLAYER_SIZE, true);                  // 2) Non-player attributes (append)
    this->write("staff.dat", Player::db, DB_PLAYER_SIZE, true);                         // 3) Player attributes (append)
    this->write("staff.dat", StaffPreferences::db, DB_STAFF_PREFERENCES_SIZE, true);    // 4) Staff preferences (append)
    this->write("staff_comp.dat", StaffComp::db, DB_STAFF_COMP_SIZE);
    this->write("staff_comp_history.dat", StaffCompHistory::db, DB_STAFF_COMP_HISTORY_SIZE);
    this->write("staff_history.dat", StaffHistory::db, DB_STAFF_HISTORY_SIZE);

    m_Progress->complete();
    delete m_Progress;
    return this->errorCheck();
}
