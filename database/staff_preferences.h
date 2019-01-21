#ifndef STAFF_PREFERENCES_H
#define STAFF_PREFERENCES_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/data_types.h"
#include "../data_types/ptr_club.h"
#include "../data_types/ptr_staff.h"

// --- Staff.dat - Staff preferences --- //
class StaffPreferences
{
private:
    long     ID;
    PtrClub  FavouriteClubs1;
    PtrClub  FavouriteClubs2;
    PtrClub  FavouriteClubs3;
    PtrClub  DislikedClubs1;
    PtrClub  DislikedClubs2;
    PtrClub  DislikedClubs3;
    PtrStaff FavouriteStaff1;
    PtrStaff FavouriteStaff2;
    PtrStaff FavouriteStaff3;
    PtrStaff DislikedStaff1;
    PtrStaff DislikedStaff2;
    PtrStaff DislikedStaff3;

public:
    // Constructor
    StaffPreferences();

    // Container
    static QVector<StaffPreferences> db;

    // Friend class
    friend class TransferImporter;
};

#endif // STAFF_PREFERENCES_H
