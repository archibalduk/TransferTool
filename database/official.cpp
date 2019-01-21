#include "official.h"

// --- Static data --- //
QVector<Official> Official::db;


/* ======================== */
/*      Officials Data      */
/* ======================== */

// --- Default constructor --- //
Official::Official()
{
}


/* ========================= */
/*      Data Validation      */
/* ========================= */

// --- Validate data --- //
void Official::validate()
{
    const int sz = db.size();

    for(int i = 0; i < sz; ++i) {
        // Validate year of birth
        db[i].YearOfBirth.validate(db[i].DateOfBirth.year());

        // Name counters
        db[i].FirstName.increment();
        db[i].SecondName.increment();
    }
}
