#ifndef CLUB_COMP_HISTORY_H
#define CLUB_COMP_HISTORY_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/data_types.h"
#include "../data_types/ptr_club.h"
#include "../data_types/ptr_club_comp.h"

// --- Club_comp_history.dat --- //
class ClubCompHistory
{
private:
    int ID;
    PtrClubComp Comp;
    short Year;
    PtrClub Winners;
    PtrClub RunnersUp;
    PtrClub ThirdPlaced;
    PtrClub Hosts;

public:
    // Constructor
    ClubCompHistory();

    // Containers (domestic & international)
    static QVector<ClubCompHistory> dbDom;
    static QVector<ClubCompHistory> dbInt;
};

#endif // CLUB_COMP_HISTORY_H
