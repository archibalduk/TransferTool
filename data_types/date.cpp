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

    // The call below handles the case when the year before adjustment was leap year and the year
    // after adjustment isn't (or vice-versa), otherwise it would incorrectly
    // convert a date of 21.05.1985 to 20.05.1968 for example, because the number
    // of days after january 1st would remain the same, and would incorrectly invert the m_LeapYear too:
    validate();
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

// --- Validate a date --- //
void Date::validate()
{
    if(m_LeapYear < 0 || m_LeapYear > 1) {
        m_LeapYear = 0;
    }

    /*Sometimes the isLeapYear 1/0 value coming from the SI database is inverted,
    i.e. sometimes it comes as 1 for years like 1965 which should come as 0,
    and sometimes it comes as 0 for years like 1968 which should come as 1,
    AND most importantly: when it comes inverted, the 'day' value coming from the SI
    database also comes wrong: it comes incorrectly decremented by 1 when the leapYear
    comes incorrectly as 0, and it comes incorrectly incremented by 1 when the leapYear
    comes incorrectly as 1, if the month is march or after, as in examples below:

      Player dusan kuciak date of birth in database is 21.05.1968
      but he comes with days 140 and leap year 0 from database while
      the expected is 141 and leap year 1 what incorrectly gives him
      a DOB of 20.05.1968 in this tool

      Player martin kuciak date of birth in database is 15.03.1965
      but he comes with days 74 and leap year 1 from database while
      the expected is 73 and leap year 0 what incorrectly gives him
      a DOB of 16.03.1968 in this tool

    I dont know if it is caused by a wrong value of isLeapYear field in the SI data itself
    or by a bug when reading this field from the SI data. Anyway, the code below solves this
    problem by incrementing/decrementing the m_Day variable when the leap year came wrong.

    I didn't test if this bug occurs in special cases like december 31 or march 1st or
    feb 29/28/27 or january 1st as I didnt find an example in the database of those cases
    to test, but this code probably fixes this bug in those cases too.
    */
    if(m_LeapYear == 1 && !QDate::isLeapYear(m_Year)) {
        //the leapYear came incorrectly as 1.
        m_LeapYear = 0;
        if(m_Day > 58){
            //the month is march or after, so we decrement the day by 1 as it came incorrect
            m_Day--;
        }
    } else if(m_LeapYear == 0 && QDate::isLeapYear(m_Year)) {
        //the leapYear came incorrectly as 0.
        m_LeapYear = 1;
        if(m_Day > 58){
            //the month is march or after, so we increment the day by 1 as it came incorrect
            m_Day++;
        }
    }

    if(m_Day < 0){
        m_Day = 0;
    } else if (m_Day > 364 && m_LeapYear == 0){
        m_Day = 364;
    } else if (m_Day > 365 && m_LeapYear == 1){
        m_Day = 365;
    }
}
