#ifndef CITY_H
#define CITY_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/city_weather.h"
#include "../data_types/data_types.h"
#include "../data_types/ptr_nation.h"
#include "../data_types/string.h"
#include "../spreadsheet/spreadsheet.h"

// --- City.dat --- //
class City
{
private:
    int         ID;
    char        Name[ SHORT_TEXT_LENGTH ];
    char        GenderName;
    PtrNation   Nation;
    double      Latitude;
    double      Longitude;
    char        Attraction;
    int         Weather;
    CityWeather WeatherCity;

public:
    // Constructor
    City();

    // Container
    static QVector<City> db;

    // Export data
    static bool exportData(Spreadsheet &s);

    // Get data
    QString getName();
};

#endif // CITY_H
