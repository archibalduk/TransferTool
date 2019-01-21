#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QChar>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QVariant>
#include <QVector>

// --- CSV spreadsheet --- //
class Spreadsheet
{
private:
    // Data
    QVector<QVector<QString>> m_Header;
    QVector<QVector<QString>> m_Data;

    // Delimiter / separator
    static QChar s_Delimiter;
    static QChar getSystemDelimiter();

    // Errors
    int showBadRowsError(const QVector<int> &badRows);

    // File I/O (functions)
    void loadRepairStrings(QVector<QString> &repairSearch, QVector<QString> &repairReplace);
    bool saveDialog(QString &path, const QString &filename = "");

    // File I/O (members)
    static int s_ReadBufferSize;
    const int m_WriteBufferSize;
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

    // Buffers
    static int readBufferSize();
    static void setReadBufferSize(const int &size);

    // Get data
    const QVector<QVector<QString> > *data();
    QString matchDateString(unsigned int row, unsigned int col);
    QString matchString(unsigned int row, unsigned int col);
    int number(unsigned int row, unsigned int col);
    QVector<QString> *row(unsigned int row);
    QString string(unsigned int row, unsigned int col);
    QVariant variant(unsigned int row, unsigned int col);

    // Set data
    void set(unsigned int &row, unsigned int col, const int &value);

    // File I/O
    QString getFilePath();
    bool open(QString path = "", unsigned int headerCount = 1, const bool &errorChecking = false);
    bool read(QString &path, unsigned int headerCount = 1, const bool &errorChecking = false);
    bool create(QString path = "", const QString &filename = "");
    bool save(QString path = "");
    bool write(QString path = "");

    // Header
    void addHeader(const QStringList &list);
    void addHeader(const QString col1);
    void addHeader(const QString col1, const QString col2);
    void addHeader(QString value, unsigned int row);
    const QVector<QVector<QString> > *header();
    QVector<QString> *header(unsigned int row);
    QString headerString(unsigned int row, unsigned int col);
    QVariant headerVariant(unsigned int row, unsigned int col);

    // Progress display
    void setSilent(bool silent);

    // Timer
    QTime time();

    // Flags
    enum ENUM_FLAGS {
        NOT_A_NUMBER = -999999999,
        BUFFER_MARGIN = 5
    };
};

#endif // SPREADSHEET_H
