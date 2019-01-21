#ifndef VLOOKUP_HASH_H
#define VLOOKUP_HASH_H

// Qt headers
#include <QHash>
#include <QString>
#include <QVector>

// Custom headers
#include "../spreadsheet/spreadsheet.h"

// --- Additional VLookup Hash --- //
class VLookupHash
{
private:
    // Counts
    unsigned int m_MatchedCount;
    unsigned int m_UnmatchedCount;

    // Default data
    void initDefaultData(Spreadsheet &s);
    void initDefaultClubData(Spreadsheet &/*s*/);
    void initDefaultClubPrefixSuffixData(Spreadsheet &s);
    void initDefaultNationData(Spreadsheet &s);

    // File I/O
    QString m_FileName;

    // Underlying data
    //QHash<QString, int> *m_Hash;
    //QVector<QString> *m_Vector;

    // Lookup type
    char m_Type;

    // Match flag
    enum ENUM_MATCH_FLAG {
        NO_MATCH = -99999999
    };

    // Spreadsheet columns
    enum ENUM_SPREADSHEET_COLUMNS {
        PREFIX_SUFFIX = 0,
        FIND = 0,
        REPLACE
    };

public:
    // Lookup types
    enum ENUM_LOOKUP_TYPES {
        NONE,
        CLUBS,
        CLUB_COMPS,
        NATIONS,
        CLUBS_PREFIX_SUFFIX
    };

    // Constructor
    VLookupHash(QString filename, char type = NONE);
    VLookupHash(char type);

    // File I/O
    void load(QHash<QString, int> &hash);
    void load(QVector<QString> &vector);

    // Lookup
    int lookupPrefixSuffix(const QString &targetString,
                           QVector<QString> &vectorPrefixSuffix, QHash<QString, int> &hash,
                           const int noMatchValue);

    // Set data
    void set(QString filename, char type);
    void set(char type);
};

#endif // VLOOKUP_HASH_H
