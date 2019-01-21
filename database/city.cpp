#include "city.h"

// --- Static data --- //
QVector<City> City::db;


/* =================== */
/*      City Data      */
/* =================== */

// --- Default constructor --- //
City::City()
{

}


/* ===================== */
/*      Export Data      */
/* ===================== */

// --- Export data --- //
bool City::exportData(Spreadsheet &s)
{
    // Header
    QStringList header;
    header << "Name" << "Nation" << "Longitude" << "Latitude";
    s.addHeader(header);

    // Rows
    for(City &itr : db)
    {
        QStringList row;
        row << itr.getName() << itr.Nation.getSafeText()
            << QString::number(itr.Longitude) << QString::number(itr.Latitude);
        s.add(row);
    }

    return true;
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Get name text --- //
QString City::getName()
{
    return String(Name).get();
}
