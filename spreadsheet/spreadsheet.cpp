#include "spreadsheet.h"
#include "../data_types/string.h"
#include "../progress_window.h"

// Qt headers
#include <QFile>
#include <QFileDialog>
#include <QLocale>
#include <QMessageBox>
#include <QRegExp>
#include <QSettings>
#include <QStringRef>
#include <QTextCodec>
#include <QTextStream>

// --- Static data --- //
QChar Spreadsheet::s_Delimiter = Spreadsheet::getSystemDelimiter();
int Spreadsheet::s_ReadBufferSize = 95000 + Spreadsheet::BUFFER_MARGIN;

/* ===================== */
/*      Spreadsheet      */
/* ===================== */

// --- Default constructor --- //
Spreadsheet::Spreadsheet(bool repairColumns) :
    m_WriteBufferSize(150),
    m_HideProgress(false),
    m_Repair(repairColumns)
{
    // Load default path setting
    QSettings settings;
    m_FilePath = settings.value("spreadsheet_path", "C:\\").toString();
}

// --- Default destructor --- //
Spreadsheet::~Spreadsheet()
{
    // Save default path setting
    QSettings settings;
    settings.setValue("spreadsheet_path", m_FilePath);
}


/* ================== */
/*      Add Data      */
/* ================== */

// --- Add a QStringList as a new row of data --- //
void Spreadsheet::add(const QStringList &list)
{
    QVector<QString> tmp;

    const int size = list.size();
    for(int i = 0; i < size; ++i)
        tmp.push_back(list[i]);

    m_Data.push_back(tmp);
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
    QVector<QString> tmp;
    tmp.push_back(col1);

    m_Data.push_back(tmp);
}

// --- Add two columns to a new row (for convenience but slower than using the QStringList method) --- //
void Spreadsheet::add(const QString col1, const QString col2)
{
    QVector<QString> tmp;
    tmp.push_back(col1);
    tmp.push_back(col2);

    m_Data.push_back(tmp);
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


/* ================ */
/*      Buffer      */
/* ================ */

// --- Get row buffer size --- //
int Spreadsheet::readBufferSize()
{
    return s_ReadBufferSize - BUFFER_MARGIN; // Discount buffer margin
}

// --- Set row buffer size when reading --- //
void Spreadsheet::setReadBufferSize(const int &size)
{
    s_ReadBufferSize = size + BUFFER_MARGIN; // Allow for a small margin of error
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Pointer to data vector --- //
const QVector<QVector<QString>> *Spreadsheet::data()
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
QVector<QString> *Spreadsheet::row(unsigned int row)
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


/* ================ */
/*      Errors      */
/* ================ */

// --- Show bad rows error message --- //
int Spreadsheet::showBadRowsError(const QVector<int> &badRows)
{
    const int size = badRows.size();

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(QString("Unexpected characters found in %1 row(s).").arg(size));
    msgBox.setInformativeText("This suggests the presence of html code and/or corrupt rows. "
                              "Importing the data may cause the application to crash or for unintended data to be inserted "
                              "into the Championship Manager database.\n\n"
                              "Click on 'Show Details...' below for a list of affected row(s).");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    // Add details of affected rows to the message box
    QString list;
    for(int i = 0; i < size; ++i)
        list.append(QString("%1\n").arg(badRows[i]));

    list.chop(1);   // Remove trailing newline character
    msgBox.setDetailedText(list);

    return msgBox.exec();
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

    // Read the rows
    while(!f.atEnd()) {
        QString row = QString::fromLatin1(f.readLine());

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
bool Spreadsheet::open(QString path, unsigned int headerCount, const bool &errorChecking)
{
    //qDebug() << "Spreadsheet::open" << path;

    path = QFileDialog::getOpenFileName(0, "Open Comma/Tab Separated File",
                                        path,
                                        "Spreadsheet (*.csv *.txt)");

    // Abort if the user has clicked Cancel
    if(path.isEmpty())
        return false;

    // Read the file
    m_Time.start();
    return this->read(path, headerCount, errorChecking);
}

// --- Read file --- //
bool Spreadsheet::read(QString &path, unsigned int headerCount, const bool &errorChecking)
{
    m_FilePath = path;

    // Open file
    QFile f(m_FilePath);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    // Detect the file delimiter
    QString dSample = f.readLine();
    int comma = dSample.count(',');
    int semicolon = dSample.count(';');
    int tab = dSample.count('\t');

    // Set delimiter to comma or semicolon according to which appears the most in the first line
    QChar d = (comma > semicolon) ? ',' : ';';

    // Use tab as a delimiter if there are more tabs than commas and semicolons
    if(tab > (comma + semicolon))
        d = '\t';

    // Return the stream to the beginning of the file
    f.seek(0);

    // Row counter
    unsigned int count = 0;

    // Progress dialog
    ProgressWindow p;
    p.hideWindow(m_HideProgress);
    p.set("Loading spreadsheet", f.size());

    // Counters (for reserving sizes of QVector and QStringList
    m_Data.reserve(s_ReadBufferSize);

    // Error checking
    const QRegExp badChars("[<>]");
    QVector<int> badRows;

    // The conditional statement below could alternatively be placed within the while loop as this would make the code tidier
    // and easier to maintain. But to keep the csv parsing as fast as possible (which is important when dealing with large
    // files), it is better to place the while loop within the conditional statement.

    // Parse the csv (no column repair)
    if(!m_Repair) {
        while(!f.atEnd()) {
            QString row = QString::fromLatin1(f.readLine());

            // Pointer to the relevant string list
            QVector<QVector<QString>> *data = (count < headerCount) ? &m_Header : &m_Data;

            // Remove any text qualifiers (i.e. speech marks)
            row.remove(QChar(0x22));

            // Check for bad characters
            if(row.contains(badChars))
                badRows.push_back(count+1);

            // Skip empty rows
            if(row.isEmpty())
            {
                ++count;
                continue;
            }

            // Parse the row and add it to the string list
            QStringList list;
            list = row.split(d);

            // Convert the list to a QVector<QString>
            QVector<QString> tmp;
            tmp.reserve(list.size()+10);    // Add ten for any additional columns to be added later

            const int size = list.size();
            for(int i = 0; i < size; ++i)
                tmp.push_back(list[i]);

            // Remove end line marker from end of final cell
            tmp[size-1].chop(1);

            data->push_back(tmp);

            // Increment the progress dialog
            p.setSteps(f.pos());

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

        while(!f.atEnd()) {
            QString row = QString::fromLatin1(f.readLine());

            // Check for any repairs
            const int size = repairSearch.size();
            for(int i = 0; i < size; ++i) {
                row.replace(repairSearch[i], repairReplace[i], Qt::CaseInsensitive);
            }

            // Pointer to the relevant string list
            QVector<QVector<QString>> *data = (count < headerCount) ? &m_Header : &m_Data;

            // Remove any text qualifiers (i.e. speech marks)
            row.remove(QChar(0x22));

            // Check for bad characters
            if(row.contains(badChars))
                badRows.push_back(count+1);

            // Skip empty rows
            if(row.isEmpty())
            {
                ++count;
                continue;
            }

            // Parse the row and add it to the string list
            QStringList list;
            list = row.split(d);

            // Convert the list to a QVector<QString>
            QVector<QString> tmp;
            tmp.reserve(list.size()+10);    // Add ten for any additional columns to be added later

            const int sz = list.size();
            for(int i = 0; i < sz; ++i)
                tmp.push_back(list[i]);

            // Remove end line marker from end of final cell
            tmp[sz-1].remove("\n");

            data->push_back(tmp);

            // Increment the progress dialog
            p.setSteps(f.pos());

            // Increment the row counter
            ++count;
        }
    }

    p.complete();
    f.close();

    if(errorChecking && badRows.size() > 0)
        this->showBadRowsError(badRows);

    return true;
}

// --- Create file/buffer via a dialog window but do not write any data to the hard drive --- //
bool Spreadsheet::create(QString path, const QString &filename)
{
    return this->saveDialog(path, filename);
}

// --- Save file via a dialog window --- //
bool Spreadsheet::saveDialog(QString &path, const QString &filename)
{
    // Use existing path if passed path is empty
    if(path.isEmpty())
        path = m_FilePath;

    // Append default file name
    if(!filename.isEmpty())
    {
        path.append("\\");
        path.append(filename);
    }

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
        QVector<QVector<QString>> *data = (i == 0) ? &m_Header : &m_Data;

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
            if(row % m_WriteBufferSize == 0)
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
    QVector<QString> tmp;

    const int size = list.size();
    for(int i = 0; i < size; ++i)
        tmp.push_back(list[i]);

    m_Header.push_back(tmp);
}

// --- Add one column to a new header row (for convenience but slower than using the QStringList method) --- //
void Spreadsheet::addHeader(const QString col1)
{
    QVector<QString> tmp;
    tmp.push_back(col1);

    m_Header.push_back(tmp);
}

// --- Add two columns to a new header row (for convenience but slower than using the QStringList method) --- //
void Spreadsheet::addHeader(const QString col1, const QString col2)
{
    QVector<QString> tmp;
    tmp.push_back(col1);
    tmp.push_back(col2);

    m_Header.push_back(tmp);
}

// --- Add string value as a new column to a header row --- //
void Spreadsheet::addHeader(QString value, unsigned int row)
{
    if(row < static_cast<unsigned int>(m_Header.size()))
        m_Header[row] << value;
}

// --- Pointer to header vector --- //
const QVector<QVector<QString>> *Spreadsheet::header()
{
    return &m_Header;
}

// --- Pointer to header row string list --- //
QVector<QString> *Spreadsheet::header(unsigned int row)
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
