#include "spreadsheet.h"
#include "../data_types/string.h"
#include "../progress_window.h"

// Qt headers
#include <QFile>
#include <QFileDialog>
#include <QLocale>
#include <QStringRef>
#include <QTextCodec>
#include <QTextStream>

#include <QDebug>

// --- Static data --- //
const int Spreadsheet::s_BufferSize = 150;
QChar Spreadsheet::s_Delimiter = Spreadsheet::getSystemDelimiter();


/* ===================== */
/*      Spreadsheet      */
/* ===================== */

// --- Default constructor --- //
Spreadsheet::Spreadsheet(bool repairColumns) :
    m_HideProgress(false),
    m_Repair(repairColumns)
{

}

// --- Default destructor --- //
Spreadsheet::~Spreadsheet()
{

}


/* ================== */
/*      Add Data      */
/* ================== */

// --- Add a QStringList as a new row of data --- //
void Spreadsheet::add(const QStringList &list)
{
    m_Data.push_back(list);
}

// --- Add a variable number of QStrings as a new row (C++11 variadic function) --- //
/*void Spreadsheet::add(const QString values...)
{
    QStringList list;

    const int count = sizeof(values);

    va_list(args);
    va_start(args, count);

    for(int i = 0; i < count; ++i) {
        list << va_arg(args, QString);
    }

    va_end(args);

    m_Data.push_back(list);
}*/

// --- Add one column to a new row (for convenience but slower than using the QStringList method) --- //
void Spreadsheet::add(const QString col1)
{
    QStringList list;
    list << col1;
    m_Data.push_back(list);
}

// --- Add two columns to a new row (for convenience but slower than using the QStringList method) --- //
void Spreadsheet::add(const QString col1, const QString col2)
{
    QStringList list;
    list << col1 << col2;
    m_Data.push_back(list);
}

// --- Add integer value as a new column to a row --- //
void Spreadsheet::add(unsigned int &row, const int &value)
{
    if(row < static_cast<unsigned int>(m_Data.size()))
        m_Data[row] << QString::number(value);
}

// --- Add string value as a new column to a row --- //
void Spreadsheet::add(unsigned int &row, QString value)
{
    if(row < static_cast<unsigned int>(m_Data.size()))
        m_Data[row] << value;
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Pointer to data vector --- //
const QVector<QStringList> *Spreadsheet::data()
{
    return &m_Data;
}

// --- Row cell date string formatted for matching against --- //
QString Spreadsheet::matchDateString(unsigned int row, unsigned int col)
{
    if(row < static_cast<unsigned int>(m_Data.size()) && col < static_cast<unsigned int>(m_Data[row].size()))
        return String(m_Data[row][col]).getMatchDateString();
    else
        return QString();
}


// --- Row cell string formatted for matching against --- //
QString Spreadsheet::matchString(unsigned int row, unsigned int col)
{
    if(row < static_cast<unsigned int>(m_Data.size()) && col < static_cast<unsigned int>(m_Data[row].size()))
        return String(m_Data[row][col]).getMatchString();
    else
        return QString();
}

// --- Row cell number --- //
int Spreadsheet::number(unsigned int row, unsigned int col)
{
    // Out of range
    if(row >= static_cast<unsigned int>(m_Data.size()))
        return NOT_A_NUMBER;

    // Not a number
    else if(m_Data[row][col].isEmpty() || m_Data[row][col] == "-")  // If empty or contains only a dash
        return NOT_A_NUMBER;

    // Return as an integer
    else
        return m_Data[row][col].toInt();
}

// --- Pointer to row string list --- //
QStringList *Spreadsheet::row(unsigned int row)
{
    if(row < static_cast<unsigned int>(m_Data.size()))
        return &m_Data[row];
    else
        return nullptr;
}

// --- Row cell string --- //
QString Spreadsheet::string(unsigned int row, unsigned int col)
{
    if(row < static_cast<unsigned int>(m_Data.size()) && col < static_cast<unsigned int>(m_Data[row].size()))
        return m_Data[row][col];
    else
        return QString();
}

// --- Row cell variant --- //
QVariant Spreadsheet::variant(unsigned int row, unsigned int col)
{
    if(row < static_cast<unsigned int>(m_Data.size()) && col < static_cast<unsigned int>(m_Data[row].size()))
        return m_Data[row][col];
    else
        return QVariant();
}


/* ================== */
/*      Set Data      */
/* ================== */

// --- Set integer value of a column of a row --- //
void Spreadsheet::set(unsigned int &row, unsigned int col, const int &value)
{
    // Abort if invalid row
    if(row >= static_cast<unsigned int>(m_Data.size()))
        return;

    // Update existing column if it already exists
    if(col < static_cast<unsigned int>(m_Data[row].size()))
        m_Data[row][col] = QString::number(value);
    // Add if the column does not exist
    else
        m_Data[row] << QString::number(value);
}


/* =================== */
/*      Delimiter      */
/* =================== */

// --- Get standard delimiter based upon the system locale --- //
QChar Spreadsheet::getSystemDelimiter()
{
    QLocale locale;
    return (locale.decimalPoint() == '.') ? ',' : ';';
}


/* ================== */
/*      File I/O      */
/* ================== */

// --- Get file path --- //
QString Spreadsheet::getFilePath()
{
    int pos = m_FilePath.lastIndexOf("/");
    return m_FilePath.left(pos+1);
}

// --- Load delimiter repair list --- //
void Spreadsheet::loadRepairStrings(QVector<QString> &repairSearch, QVector<QString> &repairReplace)
{
    // Open file
    QFile f("delimiter_repair.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    // Text stream (for reading in the text from the file)
    QTextStream s;
    s.setCodec(QTextCodec::codecForName("Latin-1"));    // ASCII
    s.setDevice(&f);

    // Read the rows
    while(!s.atEnd()) {
        QString row = s.readLine();

        // Search string
        repairSearch.push_back(row);

        // Replacement string (i.e. string without the delimiter)
        row.remove(s_Delimiter);
        repairReplace.push_back(row);
    }

    f.close();
    return;
}

// --- Open file via a dialog window --- //
bool Spreadsheet::open(QString path, unsigned int headerCount)
{
    qDebug() << "Spreadsheet::open" << path;

    path = QFileDialog::getOpenFileName(0, "Open Comma/Tab Separated File",
                                        path,
                                        "Spreadsheet (*.csv *.txt)");

    // Abort if the user has clicked Cancel
    if(path.isEmpty())
        return false;

    // Read the file
    m_Time.start();
    return this->read(path, headerCount);
}

// --- Read file --- //
bool Spreadsheet::read(QString &path, unsigned int headerCount)
{
    m_FilePath = path;

    // Open file
    QFile f(m_FilePath);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    // Text stream (for reading in the text from the file)
    QTextStream s;
    s.setCodec(QTextCodec::codecForName("Latin-1"));    // ASCII
    s.setDevice(&f);

    // Detect the file delimiter
    QString dSample = s.readLine();
    int comma = dSample.count(',');
    int semicolon = dSample.count(';');
    int tab = dSample.count('\t');

    // Set delimiter to comma or semicolon according to which appears the most in the first line
    QChar d = (comma > semicolon) ? ',' : ';';

    // Use tab as a delimiter if there are more tabs than commas and semicolons
    if(tab > (comma + semicolon))
        d = '\t';

    // Return the stream to the beginning of the file
    s.seek(0);

    // Row counter
    unsigned int count = 0;

    // Progress dialog
    ProgressWindow p;
    p.hideWindow(m_HideProgress);
    p.set("Loading spreadsheet", f.size());

    // Counters (for reserving sizes of QVector and QStringList
    int columnReserve = 50; // QStringList
    int rowReserve = 20000; // QVector
    m_Data.reserve(rowReserve);

    // The conditional statement below could alternatively be placed within the while loop as this would make the code tidier
    // and easier to maintain. But to keep the csv parsing as fast as possible (which is important when dealing with large
    // files), it is better to place the while loop within the conditional statement.

    // Parse the csv (no column repair)
    if(!m_Repair) {
        while(!s.atEnd()) {
            QString row = s.readLine();

            // Pointer to the relevant string list
            QVector<QStringList> *data = (count < headerCount) ? &m_Header : &m_Data;

            // Remove any text qualifiers (i.e. speech marks)
            row.remove(QChar(0x22));

            // Parse the row and add it to the string list
            QStringList list;
            list.reserve(columnReserve);
            list = row.split(d);
            data->append(list);

            // Update column counter
            if(list.size() > columnReserve)
                columnReserve = list.size();

            // Increment the progress dialog
            p.setSteps(s.pos());

            // Increment the row counter
            ++count;
        }
    }
    // Parse the csv (with column repair)
    else {
        // Initialise the column repair vector
        QVector<QString> repairSearch;
        QVector<QString> repairReplace;
        this->loadRepairStrings(repairSearch, repairReplace);

        while(!s.atEnd()) {
            QString row = s.readLine();

            // Check for any repairs
            const int size = repairSearch.size();
            for(int i = 0; i < size; ++i) {
                row.replace(repairSearch[i], repairReplace[i], Qt::CaseInsensitive);
            }

            // Pointer to the relevant string list
            QVector<QStringList> *data = (count < headerCount) ? &m_Header : &m_Data;

            // Remove any text qualifiers (i.e. speech marks)
            row.remove(QChar(0x22));

            // Parse the row and add it to the string list
            QStringList list;
            list.reserve(columnReserve);
            list = row.split(d);
            data->append(list);

            // Update column counter
            if(list.size() > columnReserve)
                columnReserve = list.size();

            // Increment the progress dialog
            p.setSteps(s.pos());

            // Increment the row counter
            ++count;
        }
    }

    p.complete();
    f.close();
    return true;
}

// --- Create file/buffer via a dialog window but do not write any data to the hard drive --- //
bool Spreadsheet::create(QString path)
{
    return this->saveDialog(path);
}

// --- Save file via a dialog window --- //
bool Spreadsheet::saveDialog(QString &path)
{
    // Use existing path if passed path is empty
    if(path.isEmpty())
        path = m_FilePath;

    path = QFileDialog::getSaveFileName(0, "Save Comma/Tab Separated File",
                                        path,
                                        "Spreadsheet (*.csv *.txt)");

    // Abort if the user has clicked Cancel
    if(path.isEmpty())
        return false;
    else {
        m_FilePath = path;
        return true;
    }
}
// --- Save file via a dialog window and write the data to the hard drive --- //
bool Spreadsheet::save(QString path)
{
    if(saveDialog(path) == true) {
        m_Time.start();
        return this->write();
    }
    else
        return false;
}

// --- Write file --- //
bool Spreadsheet::write(QString path)
{
    if(!path.isEmpty())
        m_FilePath = path;

    // Create file
    QFile f(m_FilePath);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;

    // Text stream (for writing text to the file)
    QTextStream s;
    s.setCodec(QTextCodec::codecForName("Latin-1"));    // ASCII
    s.setDevice(&f);

    // Progress dialog
    ProgressWindow p;
    p.hideWindow(m_HideProgress);
    p.set("Saving spreadsheet", m_Header.size() + m_Data.size());

    // Write the csv (iterate first over header and then over data strings)
    for(int i = 0; i < 2; ++i) {

        // Pointer to the relevant string list
        QVector<QStringList> *data = (i == 0) ? &m_Header : &m_Data;

        // Iterate over each row (index-based iteration should be the fastest way of writing, albeit the code is a little harder to read)
        const int rowSize = data->size();
        for(int row = 0; row < rowSize; ++row) {

            // Iterate over each cell
            const int colSize = data->at(row).size()-1; // Subtract 1 as the final element will be added after the loop
            for(int col = 0; col < colSize; ++col) {
                s << data->at(row).at(col) << s_Delimiter;
            }

            // Add final cell
            s << data->at(row).at(colSize) << "\n";

            // Flush buffer to the file every x rows
            if(row % s_BufferSize == 0)
                s.flush();

            // Increment the counter
            p.incrementStep();
        }
    }

    p.complete();
    f.close();
    return true;
}


/* ================ */
/*      Header      */
/* ================ */

// --- Add a QStringList as a new header row of data --- //
void Spreadsheet::addHeader(const QStringList &list)
{
    m_Header.push_back(list);
}

// --- Add one column to a new header row (for convenience but slower than using the QStringList method) --- //
void Spreadsheet::addHeader(const QString col1)
{
    QStringList list;
    list << col1;
    m_Header.push_back(list);
}

// --- Add two columns to a new header row (for convenience but slower than using the QStringList method) --- //
void Spreadsheet::addHeader(const QString col1, const QString col2)
{
    QStringList list;
    list << col1 << col2;
    m_Header.push_back(list);
}

// --- Add string value as a new column to a header row --- //
void Spreadsheet::addHeader(QString value, unsigned int row)
{
    if(row < static_cast<unsigned int>(m_Header.size()))
        m_Header[row] << value;
}

// --- Pointer to header vector --- //
const QVector<QStringList> *Spreadsheet::header()
{
    return &m_Header;
}

// --- Pointer to header row string list --- //
QStringList *Spreadsheet::header(unsigned int row)
{
    if(row < static_cast<unsigned int>(m_Header.size()))
        return &m_Header[row];
    else
        return nullptr;
}

// --- Header cell string --- //
QString Spreadsheet::headerString(unsigned int row, unsigned int col)
{
    if(row < static_cast<unsigned int>(m_Header.size()) && col < static_cast<unsigned int>(m_Header[row].size()))
        return m_Header[row][col];
    else
        return QString();
}

// --- Header cell variant --- //
QVariant Spreadsheet::headerVariant(unsigned int row, unsigned int col)
{
    if(row < static_cast<unsigned int>(m_Header.size()) && col < static_cast<unsigned int>(m_Header[row].size()))
        return m_Header[row][col];
    else
        return QVariant();
}


/* ========================== */
/*      Progress Display      */
/* ========================== */

// --- Show/hide the progress window --- //
void Spreadsheet::setSilent(bool silent)
{
    m_HideProgress = silent;
}


/* =============== */
/*      Timer      */
/* =============== */

// --- Get timer --- //
QTime Spreadsheet::time()
{
    return m_Time;
}
