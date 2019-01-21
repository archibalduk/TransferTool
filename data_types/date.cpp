#include "date.h"
#include <QStringList>


/* ==================== */
/*      CM/SI Date      */
/* ==================== */

// --- Default Constructor --- //
Date::Date() :
    m_Day(0),
    m_Year(0),
    m_LeapYear(0)
{
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Get as QDate --- //
QDate Date::get()
{
    return QDate(m_Year, 1, 1).addDays(m_Day);
}

// --- Get date formatted for matching against --- //
QString Date::getMatchText(int &yearAdjustment, bool &useFullDate)
{
    // Pad single day and month digits and under-size years with leading zeros
    if(useFullDate) {
        QDate d = this->get();
        int year = d.year();

        if(year > 1900)
            year+=yearAdjustment;

        return QString("%1.%2.%3")
                .arg(d.day(), 2, 10, QChar('0'))
                .arg(d.month(), 2, 10, QChar('0'))
                .arg(year, 4, 10, QChar('0'));
    }
    else {
        int year = m_Year;

        if(year > 1900)
            year+=yearAdjustment;

        return QString::number(year);
    }
}

// --- Get year --- //
short Date::year()
{
    return m_Year;
}

/* ================== */
/*      Set Data      */
/* ================== */

// --- Set from QDate --- //
void Date::set(const QDate &date, const int yearAdjustment)
{
    m_Day = static_cast<short>(date.dayOfYear() - 1);
    m_Year = static_cast<short>(date.year()) - static_cast<short>(yearAdjustment);
    m_LeapYear = (date.isLeapYear(date.year()));
}

// --- Set from QString --- //
void Date::set(const QString &value, const int yearAdjustment)
{
    // Split the string into a list
    QStringList list = value.split(".");

    // Sanity check
    if(list.size() < 3)
        return;

    // Create a date from the list
    QDate date((list[2].toInt() - yearAdjustment),
               list[1].toInt(),
               list[0].toInt());

    // Set the date
    this->set(date);
}

// --- Set from QVariant --- //
void Date::set(const QVariant &value, const int yearAdjustment)
{
    QString tmp = value.toString().simplified();    // simplified() ensures all non-ASCII whitespace is converted to ASCII
    tmp.replace(" ", "");   // Remove any whitespace

    // Abort if the string does not contain a date with '.' separators
    if(tmp.count(".") != 2)
        return;

    // Split the string into a list
    QStringList list = tmp.split(".");

    // Create a date from the list
    QDate date((list[2].toInt() - yearAdjustment),
               list[1].toInt(),
               list[0].toInt());

    // Set the date
    this->set(date);
}
