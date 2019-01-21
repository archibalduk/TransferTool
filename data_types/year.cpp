#include "year.h"


/* ==================== */
/*      CM/SI Year      */
/* ==================== */

// --- Default Constructor --- //
Year::Year() :
    m_Year(0)
{

}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Get as short --- //
short Year::get()
{
    return m_Year;
}

// --- Get year formatted for matching against --- //
QString Year::getMatchText(int &yearAdjustment)
{
    if(m_Year > 1900)
        return QString::number(m_Year+=static_cast<short>(yearAdjustment));
    else
        return QString::number(1900);
}


/* ================== */
/*      Set Data      */
/* ================== */

// --- Validate a year according to another year --- //
void Year::validate(const short year)
{
    if(year > 1900)
        m_Year = year;
}
