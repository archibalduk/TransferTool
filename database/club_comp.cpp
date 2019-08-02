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
    // For speed, the loops are embedded within the if statement
    // (rather than embedding the if statements within a loop)

    // Long names
    if(useLongNames) {
        for(ClubComp itr : dbDom) {
            if(hash.contains(String(itr.Name).getMatchString())){
                // any competitions with duplicate long names will be set their ID as NO_MATCH in the hash to avoid wrong competition being used
                hash.insert(String(itr.Name).getMatchString(), NO_MATCH);
            } else {
                hash.insert(String(itr.Name).getMatchString(), itr.ID);
            }
        }
    }
    // Short names
    else {
        for(ClubComp itr : dbDom) {
            hash.insert(String(itr.NameShort).getMatchString(), itr.ID);
        }
    }
}
