#ifndef COLOUR_H
#define COLOUR_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/data_types.h"

// --- Colour.dat --- //
class Colour
{
private:
    int ID;
    char Name[ STANDARD_TEXT_LENGTH ];
    unsigned char RedIntensity;
    unsigned char GreenIntensity;
    unsigned char BlueIntensity;

public:    
    // Constructor
    Colour();

    // Container
    static QVector<Colour> db;
};

#endif // COLOUR_H
