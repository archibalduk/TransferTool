#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QChar>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QVariant>
#include <QVector>

/* --- BENCHMARKS ---
 *
 * Using a sample of 1,000 TransferImporter records
 *
 * QVector<QStringList>  21.743 secs (total) / 0.021 secs per record
 * QVector<QStringList*> secs (total) / secs per record
 *
 */

// --- CSV spreadsheet --- //
class Spreadsheet
{
private:
    // Data
    QVector<QStringList> m_Header;
    QVector<QStringList> m_Data;

    // Delimiter / separator
    static QChar s_Delimiter;
    static QChar getSystemDelimiter();

    // File I/O (functions)
    void loadRepairStrings(QVector<QString> &repairSearch, QVector<QString> &repairReplace);
    bool saveDialog(QString &path);

    // File I/O (members)
    static const int s_BufferSize;
    QString m_FilePath;

    // Header count
    unsigned int m_HeaderCount;

    // Progress display
    bool m_HideProgress;

    // Repair columns by removing delimiter characters within columns
    bool m_Repair;

    // Timer
    QTime m_Time;

public:
    // Constructor
    Spreadsheet(bool repairColumns = false);

    // Destructor
    ~Spreadsheet();

    // Add Data
    void add(const QStringList &list);
    //void add(const QString values...);
    void add(const QString col1);
    void add(const QString col1, const QString col2);
    void add(unsigned int &row, const int &value);
    void add(unsigned int &row, QString value);

    // Get data
    const QVector<QStringList> *data();
    QString matchDateString(unsigned int row, unsigned int col);
    QString matchString(unsigned int row, unsigned int col);
    int number(unsigned int row, unsigned int col);
    QStringList *row(unsigned int row);
    QString string(unsigned int row, unsigned int col);
    QVariant variant(unsigned int row, unsigned int col);

    // Set data
    void set(unsigned int &row, unsigned int col, const int &value);

    // File I/O
    QString getFilePath();
    bool open(QString path = "", unsigned int headerCount = 1);
    bool read(QString &path, unsigned int headerCount = 1);
    bool create(QString path = "");
    bool save(QString path = "");
    bool write(QString path = "");

    // Header
    void addHeader(const QStringList &list);
    void addHeader(const QString col1);
    void addHeader(const QString col1, const QString col2);
    void addHeader(QString value, unsigned int row);
    const QVector<QStringList> *header();
    QStringList *header(unsigned int row);
    QString headerString(unsigned int row, unsigned int col);
    QVariant headerVariant(unsigned int row, unsigned int col);

    // Progress display
    void setSilent(bool silent);

    // Timer
    QTime time();

    // Flags
    enum ENUM_FLAGS {
        NOT_A_NUMBER = -999999999
    };
};

#endif // SPREADSHEET_H
