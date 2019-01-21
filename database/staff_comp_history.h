#ifndef STAFF_COMP_HISTORY_H
#define STAFF_COMP_HISTORY_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/data_types.h"
#include "../data_types/ptr_name.h"
#include "../data_types/ptr_staff.h"

// --- Staff_comp_history.dat --- //
class StaffCompHistory
{
private:
    int ID;
    int StaffComp;
    short Year;
    PtrFirstName FirstPlacedFirstName;
    PtrSecondName FirstPlacedSecondName;
    PtrStaff FirstPlacedIndex;
    int FirstPlacedInfo;
    PtrFirstName SecondPlacedFirstName;
    PtrSecondName SecondPlacedSecondName;
    PtrStaff SecondPlacedIndex;
    int SecondPlacedInfo;
    PtrFirstName ThirdPlacedFirstName;
    PtrSecondName ThirdPlacedSecondName;
    PtrStaff ThirdPlacedIndex;
    int ThirdPlacedInfo;

public:
    // Constructor
    StaffCompHistory();

    // Container
    static QVector<StaffCompHistory> db;
};

#endif // STAFF_COMP_HISTORY_H
