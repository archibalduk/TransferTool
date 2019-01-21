#ifndef STADIUM_H
#define STADIUM_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/data_types.h"

// --- Stadium.dat --- //
class Stadium
{
private:
    int ID;
    char Name[ STANDARD_TEXT_LENGTH ];
    char GenderName;
    int City;
    int Capacity;
    int SeatingCapacity;
    int ExpansionCapacity;
    int NearbyStadium;
    char Covered;
    char UnderSoilHeating;

public:
    // Constructor
    Stadium();

    // Container
    static QVector<Stadium> db;
};

#endif // STADIUM_H
