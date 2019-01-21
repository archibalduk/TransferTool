#include "staff_preferences.h"

// --- Static data --- //
QVector<StaffPreferences> StaffPreferences::db;


/* ================================ */
/*      Staff Preferences Data      */
/* ================================ */

// --- Default constructor --- //
StaffPreferences::StaffPreferences() :
    ID(db.size())   // Allocate next ID to item
{
}
