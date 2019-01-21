#ifndef STAFF_COMP_H
#define STAFF_COMP_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/data_types.h"

// --- Staff_comp.dat --- //
class StaffComp
{
private:
    int ID;
    char Name[ STANDARD_TEXT_LENGTH ];
    char GenderName;
    char NameShort[ SHORT_TEXT_LENGTH ];
    char GenderNameShort;
    int Continent;
    int Nation;
    int ForegroundColour;
    int BackgroundColour;
    short Reputation; // Version 0x02 - Changed char->short

public:
    // Constructor
    StaffComp();

    // Container
    static QVector<StaffComp> db;
};

#endif // STAFF_COMP_H
