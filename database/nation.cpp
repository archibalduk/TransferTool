#include "nation.h"
#include "../data_types/string.h"

// --- Static data --- //
QVector<Nation> Nation::db;


/* ===================== */
/*      Nation Data      */
/* ===================== */

// --- Default constructor --- //
Nation::Nation()
{
}


/* ================== */
/*      Counters      */
/* ================== */

// --- Decrement club count --- //
void Nation::decrementClubCount()
{
    --NumberClubs;
}

// --- Decrement staff count --- //
void Nation::decrementStaffCount()
{
    --NumberStaff;
}

// --- Increment club count --- //
void Nation::incrementClubCount()
{
    ++NumberClubs;
}

// --- Increment staff count --- //
void Nation::incrementStaffCount()
{
    ++NumberStaff;
}

// --- Reset club count --- //
void Nation::resetClubCount()
{
    NumberClubs = 0;
}

// --- Reset staff count --- //
void Nation::resetStaffCount()
{
    NumberStaff = 0;
}

// --- Update all club and staff counts --- //
void Nation::resetCounters()
{
    // Reset all counts
    for(Nation &itr : db) {
        itr.NumberClubs = 0;
        itr.NumberStaff = 0;
    }
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Get name text --- //
QString Nation::getLongName()
{
    return String(Name).get();
}

// --- Get short name text --- //
QString Nation::getShortName()
{
    return String(NameShort).get();
}


/* ==================== */
/*      Match Data      */
/* ==================== */

// --- Populate a hash with nation data --- //
void Nation::createHash(QHash<QString, int> &hash, bool useLongNames)
{
    // For speed, the loops are embedded within the if statement
    // (rather than embedding the if statements within a loop)

    // Long names
    if(useLongNames) {
        for(Nation itr : db) {
            hash.insert(String(itr.Name).getMatchString(), itr.ID);
        }
    }
    // Short names
    else {
        for(Nation itr : db) {
            hash.insert(String(itr.NameShort).getMatchString(), itr.ID);
        }
    }

    // Add no nation text (text must be lowercase
    hash.insert("", NO_NATION);
    hash.insert("-", NO_NATION);
    hash.insert("_none", NO_NATION);
}
