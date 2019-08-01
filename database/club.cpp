#include "club.h"

// --- Static data --- //
QVector<Club> Club::dbDom;
QVector<Club> Club::dbInt;


/* =================== */
/*      Club Data      */
/* =================== */

// --- Default constructor --- //
Club::Club()
{

}


/* ========================= */
/*      Data Validation      */
/* ========================= */

// --- Validate data --- //
void Club::validate()
{
    const int szDom = dbDom.size();

    for(int i = 0; i < szDom; ++i) {
        // Nation counter
        dbDom[i].Nation.incrementClubCount();
    }
}


/* ===================== */
/*      Export Data      */
/* ===================== */

// --- Export data --- //
bool Club::exportData(Spreadsheet &s)
{
    // Header
    QStringList header;
    header << "Type"
           << "Long Name" << "Short Name" << "Nation"
           << "Division" << "Reserve Division"
           << "CSV Long Name" << "CSV Short Name";
    s.addHeader(header);

    // Rows (domestic)
    for(Club &itr : dbDom)
    {
        QStringList row;
        row << "Dom"; // Domestic team
        itr.exportRow(row);
        s.add(row);
    }

    // Rows (international)
    for(Club &itr : dbInt)
    {
        QStringList row;
        row << "Nat"; // National team
        itr.exportRow(row);
        s.add(row);
    }

    return true;
}

// --- Export roster data --- //
bool Club::exportRosters(Spreadsheet &s)
{
    // Header
    QStringList header;
    header << "Club";

    for(int i = 0; i < SQUAD_SIZE; ++i)
        header << ("Player " + QString::number(i));

    s.addHeader(header);

    // Rows (domestic)
    for(Club &itr : dbDom) {
        QStringList row;
        row << itr.getShortName();

        for(int i = 0; i < SQUAD_SIZE; ++i)
            row << itr.SquadList[i].getText();

        s.add(row);
    }

    // Rows (international)
    for(Club &itr : dbInt) {
        QStringList row;
        row << itr.getShortName();

        for(int i = 0; i < SQUAD_SIZE; ++i)
            row << itr.SquadList[i].getText();

        s.add(row);
    }

    return true;
}

// --- Export data for importer --- //
bool Club::exportDataForImporter(Spreadsheet &s)
{
    // Header
    QStringList header;
    header << "Club Long Name"
           << "Reputation(0-20)" << "Cash(MIN:-99999999 MAX:149999999)" << "Training(0-20)"
           << "Professional Status(0=Not Set 1=Pro 2=Semi 3=Amateur)" << "PLC(-1:Yes 0:No)"
           << "Attendance(0-999999999)" << "Min Attendance(0-999999999)"
           << "Max Attendance(0-999999999)" << "Last Position(0-50)"
           << "Last Division" << "Division" << "Reserve Division";
    s.addHeader(header);

    // Rows (domestic)
    for(Club &itr : dbDom)
    {
        QStringList row;
        itr.exportRowForImporter(row);
        s.add(row);
    }

    return true;
}

// --- Export row of club data --- //
void Club::exportRow(QStringList &row)
{
    // Text buffers
    String longName(Name);
    String shortName(NameShort);

    row << longName.get()
        << shortName.get()
        << Nation.getSafeText()
        << Division.getText()
        << ReserveDivision.getText()
        << longName.getMatchString()
        << shortName.getMatchString();
}

// --- Export row of club data for importer --- //
void Club::exportRowForImporter(QStringList &row)
{
    // Text buffers
    String longName(Name);

    row << longName.get()
        << QString::number(Reputation/500)
        << QString::number(Cash)
        << QString::number(Training)
        << QString::number(ProfessionalStatus)
        << QString::number(PLC)
        << QString::number(Attendance)
        << QString::number(MinAttendance)
        << QString::number(MaxAttendance)
        << QString::number(LastPosition)
        << LastDivision.getText()
        << Division.getText()
        << ReserveDivision.getText();
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Get name text --- //
QString Club::getLongName()
{
    return String(Name).get();
}

// --- Get short name text --- //
QString Club::getShortName()
{
    return String(NameShort).get();
}


/* ==================== */
/*      Match Data      */
/* ==================== */

// --- Populate a hash with domestic club data --- //
void Club::createHash(QHash<QString, int> &hash, bool useLongNames, bool unmatchDuplicates)
{

    // Add free agent text (text must be lowercase)
    // Only added if the hash is empty (if it is not empty, it suggests that this function
    // has been called previously and thus the free agent text has already been added)
    if(hash.isEmpty()) {
        hash.insert("-", FREE_AGENT);
        hash.insert("_none", FREE_AGENT);
        hash.insert("_fa", FREE_AGENT);
        hash.insert("_free", FREE_AGENT);
        hash.insert("_free_agent", FREE_AGENT);
        hash.insert("_free_transfer", FREE_AGENT);
    }

    // For speed, the loops are embedded within the if statement
    // (rather than embedding the if statements within a loop)   

    // Long names
    if(useLongNames) {
        for(Club itr : dbDom) {
            if(unmatchDuplicates && hash.contains(String(itr.Name).getMatchString())){
                // if unmatchDuplicates is true, any clubs with duplicate long names will be set their ID as NO_MATCH in the hash to avoid wrong club being used
                hash.insert(String(itr.Name).getMatchString(), NO_MATCH);
            } else {
                hash.insert(String(itr.Name).getMatchString(), itr.ID);
            }
        }
    }
    // Short names
    else {
        for(Club itr : dbDom) {
            hash.insert(String(itr.NameShort).getMatchString(), itr.ID);
        }
    }
}


/* ================= */
/*      Rosters      */
/* ================= */

// --- Add staff to roster --- //
bool Club::addToRoster(const PtrStaff &staff, const Job &job)
{
    const int j = static_cast<short>(job.get());

    // Abort if invalid
    if(j == Job::INVALID_JOB)
        return false;

    // Flags used for mixed player/non-player roles
    bool playerSuccess = false;
    bool nonPlayerSuccess = false;

    switch(j) {

    case Job::CHAIRMAN:
        if(Chairman.get() >= VALID)
            return false;
        else
            Chairman = staff;
        return true;

    case Job::MANAGING_DIRECTOR:
    case Job::GENERAL_MANAGER:
    case Job::DIRECTOR_OF_FOOTBALL:

        for(int i = 0; i < DIRECTOR_SIZE; ++i) {
            if(DirectorList[i].get() < VALID) {
                DirectorList[i] = staff;
                return true;
            }
        }

        return false;

    case Job::MANAGER:
        if(Manager.get() >= VALID)
            return false;
        else
            Manager = staff;
        return true;

    case Job::ASSISTANT_MANAGER:
        if(AssistantManager.get() >= VALID)
            return false;
        else
            AssistantManager = staff;
        return true;

    case Job::RESERVE_TEAM_MANAGER:
        if(Manager.get() >= VALID)
            return false;
        else
            Manager = staff;
        return true;

    case Job::COACH:

        for(int i = 0; i < COACH_SIZE; ++i) {
            if(CoachList[i].get() < VALID) {
                CoachList[i] = staff;
                return true;
            }
        }

        return false;

    case Job::SCOUT:

        for(int i = 0; i < SCOUT_SIZE; ++i) {
            if(ScoutList[i].get() < VALID) {
                ScoutList[i] = staff;
                return true;
            }
        }

        return false;

    case Job::PHYSIO:

        for(int i = 0; i < PHYSIO_SIZE; ++i) {
            if(PhysioList[i].get() < VALID) {
                PhysioList[i] = staff;
                return true;
            }
        }

        return false;

    case Job::PLAYER:

        for(int i = 0; i < SQUAD_SIZE; ++i) {
            if(SquadList[i].get() < VALID) {
                SquadList[i] = staff;
                return true;
            }
        }

        return false;

    case Job::PLAYER_MANAGER:

        if(Manager.get() >= VALID)
            return false;
        else {
            Manager = staff;
            nonPlayerSuccess = true;
        }

        for(int i = 0; i < SQUAD_SIZE; ++i) {
            if(SquadList[i].get() < VALID) {
                SquadList[i] = staff;
                playerSuccess = true;
                break;
            }
        }

        return (playerSuccess && nonPlayerSuccess);

    case Job::PLAYER_ASSISTANT_MANAGER:

        if(AssistantManager.get() >= VALID)
            return false;
        else {
            AssistantManager = staff;
            nonPlayerSuccess = true;
        }

        for(int i = 0; i < SQUAD_SIZE; ++i) {
            if(SquadList[i].get() < VALID) {
                SquadList[i] = staff;
                playerSuccess = true;
                break;
            }
        }

        return (playerSuccess && nonPlayerSuccess);

    case Job::PLAYER_RESERVE_TEAM_MANAGER:

        if(Manager.get() >= VALID)
            return false;
        else {
            Manager = staff;
            nonPlayerSuccess = true;
        }

        for(int i = 0; i < SQUAD_SIZE; ++i) {
            if(SquadList[i].get() < VALID) {
                SquadList[i] = staff;
                playerSuccess = true;
                break;
            }
        }

        return (playerSuccess && nonPlayerSuccess);

    case Job::PLAYER_COACH:

        for(int i = 0; i < COACH_SIZE; ++i) {
            if(CoachList[i].get() < VALID) {
                CoachList[i] = staff;
                nonPlayerSuccess = true;
                break;
            }
        }

        for(int i = 0; i < SQUAD_SIZE; ++i) {
            if(SquadList[i].get() < VALID) {
                SquadList[i] = staff;
                playerSuccess = true;
                break;
            }
        }

        return (playerSuccess && nonPlayerSuccess);

    case Job::PLAYER_RETIRED:
    case Job::MEDIA_PUNDIT:
        return false;

    default:
        return false;
    }

    return false;
}

// --- Clear all rosters --- //
void Club::clearRosters()
{
    // Domestic teams
    int sz = dbDom.size();

    for(int i = 0; i < sz; ++ i) {
        dbDom[i].Chairman = NO_STAFF;
        dbDom[i].Manager = NO_STAFF;
        dbDom[i].AssistantManager = NO_STAFF;

        for(int x = 0; x < DIRECTOR_SIZE; ++x)
            dbDom[i].DirectorList[x] = NO_STAFF;

        for(int x = 0; x < SQUAD_SIZE; ++x)
            dbDom[i].SquadList[x] = NO_STAFF;

        for(int x = 0; x < COACH_SIZE; ++x)
            dbDom[i].CoachList[x] = NO_STAFF;

        for(int x = 0; x < SCOUT_SIZE; ++x)
            dbDom[i].ScoutList[x] = NO_STAFF;

        for(int x = 0; x < PHYSIO_SIZE; ++x)
            dbDom[i].PhysioList[x] = NO_STAFF;
    }

    // International teams
    sz = dbInt.size();

    for(int i = 0; i < sz; ++ i) {
        dbInt[i].Chairman = NO_STAFF;
        dbInt[i].Manager = NO_STAFF;
        dbInt[i].AssistantManager = NO_STAFF;

        for(int x = 0; x < DIRECTOR_SIZE; ++x)
            dbInt[i].DirectorList[x] = NO_STAFF;

        for(int x = 0; x < SQUAD_SIZE; ++x)
            dbInt[i].SquadList[x] = NO_STAFF;

        for(int x = 0; x < COACH_SIZE; ++x)
            dbInt[i].CoachList[x] = NO_STAFF;

        for(int x = 0; x < SCOUT_SIZE; ++x)
            dbInt[i].ScoutList[x] = NO_STAFF;

        for(int x = 0; x < PHYSIO_SIZE; ++x)
            dbInt[i].PhysioList[x] = NO_STAFF;
    }
}
