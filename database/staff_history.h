#ifndef STAFF_HISTORY_H
#define STAFF_HISTORY_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/data_types.h"
#include "../data_types/ptr_club.h"
#include "../data_types/ptr_staff.h"

// --- Staff_history.dat --- //
class StaffHistory
{
private:
    int     ID;
    PtrStaff     Staff;
    short   Year;
    PtrClub     Club;
    char    OnLoan;
    unsigned char Apps;
    unsigned char Goals;

public:
    // Constructor
    StaffHistory();

    // Container
    static QVector<StaffHistory> db;
};

#endif // STAFF_HISTORY_H
