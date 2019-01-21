#include "ptr_club_comp.h"
#include "../database/club_comp.h"

// --- Get default display text --- //
QString PtrClubComp::getText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < ClubComp::dbDom.size())
        return ClubComp::dbDom[id].getLongName();
    else
        return s_OutOfRangeText;
}

// --- Set id --- //
void PtrClubComp::set(const int &i)
{
    if(i < VALID)
        id = NONE;
    else if(i < ClubComp::dbDom.size())
        id = i;
}

// --- Set id --- //
void PtrClubComp::set(const QVariant &value)
{
    if(value.toInt() < VALID)
        id = NONE;
    else if(value.toInt() < ClubComp::dbDom.size())
        id = value.toInt();
}
