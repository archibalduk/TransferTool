#include "club_comp.h"

// --- Static data --- //
QVector<ClubComp> ClubComp::dbDom;
QVector<ClubComp> ClubComp::dbInt;


/* =============================== */
/*      Club Competition Data      */
/* =============================== */

// --- Default constructor --- //
ClubComp::ClubComp()
{

}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Get name text for models --- //
QString ClubComp::getLongName()
{
    return String(Name).get();
}


/* ==================== */
/*      Match Data      */
/* ==================== */

// --- Populate a hash with domestic club comp data --- //
void ClubComp::createHash(QHash<QString, int> &hash, bool useLongNames)
{

    // Add none text (text must be lowercase)
    // Only added if the hash is empty (if it is not empty, it suggests that this function
    // has been called previously and thus the none text has already been added)
    if(hash.isEmpty()) {
        hash.insert("-", NONE);
        hash.insert("_none", NONE);
    }

    // For speed, the loops are embedded within the if statement
    // (rather than embedding the if statements within a loop)

    // Long names
    if(useLongNames) {
        for(ClubComp itr : dbDom) {
            hash.insert(String(itr.Name).getMatchString(), itr.ID);
        }
    }
    // Short names
    else {
        for(ClubComp itr : dbDom) {
            hash.insert(String(itr.NameShort).getMatchString(), itr.ID);
        }
    }
}
