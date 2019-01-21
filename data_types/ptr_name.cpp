#include "ptr_name.h"
#include "../database/name.h"


/* ===================== */
/*      Common Name      */
/* ===================== */

// --- Default constructor --- //
PtrCommonName::PtrCommonName() :
    DataPointer()
{

}

// --- Increment name counter --- //
void PtrCommonName::increment()
{
    if(id < VALID || id >= CommonName::db.size())
        return;
    else
        CommonName::db[id].incrementCount();
}

// --- Get text for matching against --- //
QString PtrCommonName::getMatchText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < CommonName::db.size())
        return String(CommonName::db[id].getName()).getMatchString();
    else
        return s_OutOfRangeText;
}

// --- Get text with no accents --- //
QString PtrCommonName::getNoAccentsText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < CommonName::db.size())
        return String(CommonName::db[id].getName()).getNoAccentsString();
    else
        return s_OutOfRangeText;
}

// --- Get CSV-friendly text --- //
QString PtrCommonName::getSafeText() const
{
    return this->getText().remove(",");
}

// --- Get default display text --- //
QString PtrCommonName::getText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < CommonName::db.size())
        return CommonName::db[id].getName();
    else
        return s_OutOfRangeText;
}

// --- Return whether id is none --- //
bool PtrCommonName::isNone() const
{
    if(id <= 0) // Id of zero for CommonName is also _none
        return true;
    else if(this->getText().isEmpty())  // Some CommonNames appear to be blank/empty (treat them as _none)
        return true;
    else
        return false;
}

// --- Set id --- //
void PtrCommonName::set(const int &i)
{
    if(i < 0)
        id = NONE;
    else if(i < CommonName::db.size())
        id = i;
}

// --- Set id --- //
void PtrCommonName::set(const QVariant &value)
{
    if(value.toInt() < 0)
        id = NONE;
    else if(value.toInt() < CommonName::db.size())
        id = value.toInt();
}

// --- Set id from string (and add if not present in the database) --- //
void PtrCommonName::set(const QString &name, const int &nation)
{
    id = CommonName::set(name, nation);
}


/* ==================== */
/*      First Name      */
/* ==================== */

// --- Default constructor --- //
PtrFirstName::PtrFirstName() :
    DataPointer()
{

}

// --- Increment name counter --- //
void PtrFirstName::increment()
{
    if(id < VALID || id >= FirstName::db.size())
        return;
    else
        FirstName::db[id].incrementCount();
}

// --- Get text for matching against --- //
QString PtrFirstName::getMatchText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < FirstName::db.size())
        return String(FirstName::db[id].getName()).getMatchString();
    else
        return s_OutOfRangeText;
}

// --- Get text with no accents --- //
QString PtrFirstName::getNoAccentsText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < FirstName::db.size())
        return String(FirstName::db[id].getName()).getNoAccentsString();
    else
        return s_OutOfRangeText;
}

// --- Get CSV-friendly text --- //
QString PtrFirstName::getSafeText() const
{
    return this->getText().remove(",");
}

// --- Get default display text --- //
QString PtrFirstName::getText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < FirstName::db.size())
        return FirstName::db[id].getName();
    else
        return s_OutOfRangeText;
}

// --- Set id --- //
void PtrFirstName::set(const int &i)
{
    if(i < 0)
        id = NONE;
    else if(i < FirstName::db.size())
        id = i;
}

// --- Set id --- //
void PtrFirstName::set(const QVariant &value)
{
    if(value.toInt() < 0)
        id = NONE;
    else if(value.toInt() < FirstName::db.size())
        id = value.toInt();
}

// --- Set id from string (and add if not present in the database) --- //
void PtrFirstName::set(const QString &name, const int &nation)
{
    id = FirstName::set(name, nation);
}


/* ===================== */
/*      Second Name      */
/* ===================== */

// --- Default constructor --- //
PtrSecondName::PtrSecondName() :
    DataPointer()
{

}

// --- Increment name counter --- //
void PtrSecondName::increment()
{
    if(id < VALID || id >= SecondName::db.size())
        return;
    else
        SecondName::db[id].incrementCount();
}

// --- Get text for matching against --- //
QString PtrSecondName::getMatchText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < SecondName::db.size())
        return String(SecondName::db[id].getName()).getMatchString();
    else
        return s_OutOfRangeText;
}

// --- Get text with no accents --- //
QString PtrSecondName::getNoAccentsText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < SecondName::db.size())
        return String(SecondName::db[id].getName()).getNoAccentsString();
    else
        return s_OutOfRangeText;
}

// --- Get CSV-friendly text --- //
QString PtrSecondName::getSafeText() const
{
    return this->getText().remove(",");
}

// --- Get default display text --- //
QString PtrSecondName::getText() const
{
    if(id < VALID)
        return s_NoneText;
    else if(id < SecondName::db.size())
        return SecondName::db[id].getName();
    else
        return s_OutOfRangeText;
}

// --- Set id --- //
void PtrSecondName::set(const int &i)
{
    if(i < 0)
        id = NONE;
    else if(i < SecondName::db.size())
        id = i;
}

// --- Set id --- //
void PtrSecondName::set(const QVariant &value)
{
    if(value.toInt() < 0)
        id = NONE;
    else if(value.toInt() < SecondName::db.size())
        id = value.toInt();
}

// --- Set id from string (and add if not present in the database) --- //
void PtrSecondName::set(const QString &name, const int &nation)
{
    id = SecondName::set(name, nation);
}
