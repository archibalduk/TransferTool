#include "ptr_nation.h"
#include "../database/nation.h"

PtrNation::PtrNation(const int i) :
    DataPointer(i)
{

}

// --- Increment club counter --- //
void PtrNation::incrementClubCount()
{
    if(id < VALID || id >= Nation::db.size())
        return;
    else
        Nation::db[id].incrementClubCount();
}

// --- Increment staff counter --- //
void PtrNation::incrementStaffCount()
{
    if(id < VALID || id >= Nation::db.size())
        return;
    else
        Nation::db[id].incrementStaffCount();
}

// --- Get CSV-friendly text --- //
QString PtrNation::getSafeText() const
{
    return this->getText().remove(",");
}

// --- Get default display text --- //
QString PtrNation::getText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < Nation::db.size())
        return Nation::db[id].getLongName();
    else
        return s_OutOfRangeText;
}

// --- Set id --- //
void PtrNation::set(const int &i)
{
    if(i < 0)
        id = NO_NATION;
    else if(i < Nation::db.size())
        id = i;
}

// --- Set id --- //
void PtrNation::set(const QVariant &value)
{
    if(value.toInt() < 0)
        id = NO_NATION;
    else if(value.toInt() < Nation::db.size())
        id = value.toInt();
}
