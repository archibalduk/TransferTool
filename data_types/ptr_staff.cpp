#include "ptr_staff.h"
#include "../database/staff.h"

PtrStaff::PtrStaff()
{

}

PtrStaff::PtrStaff(const int i) :
    DataPointer(i)
{

}

// --- Get default display text --- //
QString PtrStaff::getText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < Staff::db.size())
        return Staff::db[id].getDisplayText();
    else
        return s_OutOfRangeText;
}

// --- Set id --- //
void PtrStaff::set(const int &i)
{
    if(i < VALID)
        id = NONE;
    else if(i < Staff::db.size())
        id = i;
}

// --- Set id --- //
void PtrStaff::set(const QVariant &value)
{
    if(value.toInt() < VALID)
        id = NONE;
    else if(value.toInt() < Staff::db.size())
        id = value.toInt();
}
