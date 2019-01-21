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
