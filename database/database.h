#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QStringList>
#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

// Database headers
#include "index.h"

// UI headers
#include "progress_window.h"

// --- Database --- //
class Database
{
private:
    // Error checking
    bool errorCheck();

    // File I/O: Dat File
    template<typename T> bool read(QString filename, QVector<T> &data, int recordSize, int skipBytes = 0, int recordCount = -1);
    bool read(const QString &filename, qint32 (*fnRead)(QDataStream &, const qint32 &), int recordSize, int skipBytes = 0, int recordCount = -1);
    bool readIndex(QString filename, QVector<Index> &data, int recordSize);
    template<typename T> bool write(QString filename, QVector<T> &data, int recordSize, bool append = false);
    bool writeIndex(QString filename, QVector<Index> &data, int recordSize);

    // Progress dialog
    ProgressWindow *m_Progress;

    // Struct lengths
    enum ENUM_TABLE_SIZES {
        DB_CITY_SIZE = 56,
        DB_CLUB_SIZE = 581,
        DB_CLUB_COMP_SIZE = 107,
        DB_CLUB_COMP_HISTORY_SIZE = 26,
        DB_COLOUR_SIZE = 58,
        DB_CONTINENT_SIZE = 198,
        DB_INDEX_SIZE = 67,
        DB_NAME_SIZE = 60,
        DB_NATION_SIZE = 290,
        DB_NON_PLAYER_SIZE = 68,
        DB_OFFICIAL_SIZE = 43,
        DB_PLAYER_SIZE = 70,
        DB_STADIUM_SIZE = 78,
        DB_STAFF_SIZE = 110,
        DB_STAFF_COMP_SIZE = 101,
        DB_STAFF_COMP_HISTORY_SIZE = 58,
        DB_STAFF_HISTORY_SIZE = 17,
        DB_STAFF_PREFERENCES_SIZE = 52,
    };

    // Database file count (index.dat + 22 tables)
    enum ENUM_FILE_COUNT { FILE_COUNT = 23 };

public:
    // Constructor
    Database();
    ~Database();

    // Data validation
    void validate(ProgressWindow *p = nullptr);

    // File I/O: Database
    bool load(const QString &folder);
    bool save(const QString &folder);

    // File I/O (members)
    QStringList m_FileErrorList;
    QString m_Folder;
};

#endif // DATABASE_H
