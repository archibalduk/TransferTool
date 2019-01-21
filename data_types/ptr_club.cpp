#include "ptr_club.h"
#include "../database/club.h"

PtrClub::PtrClub() :
    DataPointer()
{

}

PtrClub::PtrClub(const int i) :
    DataPointer(i)
{

}

// --- Add to roster --- //
bool PtrClub::addToRoster(const int staffId, const Job &job)
{
    PtrStaff staff(staffId);

    if(id < VALID)
        return true;

    else if(id < Club::dbDom.size())
        return Club::dbDom[id].addToRoster(staff, job);

    else {
        int natId = id - Club::dbDom.size();

        if(natId < Club::dbInt.size())
            return Club::dbInt[natId].addToRoster(staff, job);
    }

    return false;
}

// --- Get text with no accents --- //
QString PtrClub::getNoAccentsText() const
{
    if(id < VALID)
        return s_NoneText;

    else if(id < Club::dbDom.size())
        return String(Club::dbDom[id].getLongName()).getNoAccentsString();

    else {
        int natId = id - Club::dbDom.size();

        if(natId < Club::dbInt.size())
            return String(Club::dbInt[natId].getLongName()).getNoAccentsString();
    }

    return s_OutOfRangeText;
}

// --- Get CSV-friendly text --- //
QString PtrClub::getSafeText() const
{
    return this->getText().remove(",");
}

// --- Get default display text --- //
QString PtrClub::getText() const
{
    if(id < VALID)
        return s_NoneText;

    else if(id < Club::dbDom.size())
        return Club::dbDom[id].getLongName();

    else {
        int natId = id - Club::dbDom.size();

        if(natId < Club::dbInt.size())
            return Club::dbInt[natId].getLongName();
    }

    return s_OutOfRangeText;
}

// --- Set id --- //
void PtrClub::set(const int &i)
{
    if(i < VALID)
        id = NONE;
    else if(i < Club::dbDom.size())
        id = i;
}

// --- Set id --- //
void PtrClub::set(const QVariant &value)
{
    if(value.toInt() < VALID)
        id = NONE;
    else if(value.toInt() < Club::dbDom.size())
        id = value.toInt();
}
