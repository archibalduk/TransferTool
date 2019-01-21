#ifndef CONTINENT_H
#define CONTINENT_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/data_types.h"

// --- Continent.dat --- //
class Continent
{
private:
    int ID;
    char Name[ SHORT_TEXT_LENGTH ];
    char GenderName;
    char NameThreeLetter[ 4 ];
    char NameContinentality[ SHORT_TEXT_LENGTH ];
    char FederationName[ LONG_TEXT_LENGTH ];
    char GenderFederationName;
    char FederationNameShort[ SHORT_TEXT_LENGTH ];
    char GenderFederationNameShort;
    double RegionalStrength;

public:
    // Constructor
    Continent();

    // Container
    static QVector<Continent> db;
};

#endif // CONTINENT_H
