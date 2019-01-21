#ifndef INDEX_H
#define INDEX_H

#include <QString>
#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)
#include "data_types/data_types.h"

// Forward declare model so that it can be a friend of the class
class IndexModel;

// --- Index.dat --- //
class Index
{
private:
    char Filename[ MAX_INDEX_NAME + 1 ];
    int  FileId;
    int  TableSize;
    int  Offset;
    int  Version;

public:  
    // Index header
    enum ENUM_HEADER_SIZE { INDEX_HEADER_SIZE = 8 };
    static char s_Header[INDEX_HEADER_SIZE];

    // Constructor
    Index();

    // Container
    static QVector<Index> db;

    // Get data
    QString getFileName();
    int getOffset();
    int getRecordCount();

    // Set data
    void set(const int size, const int offset = 0);

    // Friend class
    friend class IndexModel;

    enum ENUM_INDEX_POSITIONS {
        CLUB_TABLE,
        NATION_CLUBS_TABLE,
        COLOUR_TABLE,
        CONTINENT_TABLE,
        NATION_TABLE,
        OFFICIALS_TABLE,
        STADIUM_TABLE,
        STAFF_TABLE,
        //YOUTH_PLAYER_TABLE = 8, // Not used
        NON_PLAYER_TABLE,
        PLAYER_TABLE,
        STAFF_COMP_TABLE,
        CITY_TABLE,
        CLUB_COMP_TABLE,
        NATION_COMP_TABLE,
        FIRST_NAME_TABLE,
        SECOND_NAME_TABLE,
        COMMON_NAME_TABLE,
        STAFF_HISTORY_TABLE,
        STAFF_COMP_HISTORY_TABLE,
        CLUB_COMP_HISTORY_TABLE,
        NATION_COMP_HISTORY_TABLE,
        STAFF_PREFERENCES_TABLE, // MUX with youth player table
        INDEX_TABLE_COUNT
    };
};

#endif // INDEX_H
