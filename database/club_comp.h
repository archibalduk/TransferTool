#ifndef CLUB_COMP_H
#define CLUB_COMP_H

#include <QString>
#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/string.h"
#include "../data_types/data_types.h"
#include "../data_types/ptr_nation.h"

// --- Club_comp.dat --- //
class ClubComp
{
private:
    int  ID;
    char Name[ STANDARD_TEXT_LENGTH ];
    char GenderName;
    char NameShort[ SHORT_TEXT_LENGTH ];
    char GenderNameShort;
    char NameThreeLetter[ 4 ];
    char Scope;
    char Selected;
    int Continent;
    PtrNation Nation;
    int ForegroundColour;
    int BackgroundColour;
    short Reputation; // Version 0x02 - Changed char->short

public:
    // Constructor
    ClubComp();

    // Containers (domestic & international)
    static QVector<ClubComp> dbDom;
    static QVector<ClubComp> dbInt;

    // Get data
    QString getLongName();

    // Match data
    static void createHash(QHash<QString, int> &hash, bool useLongNames = true);
};

#endif // CLUB_COMP_H
