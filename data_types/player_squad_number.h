#ifndef PLAYER_SQUAD_NUMBER_H
#define PLAYER_SQUAD_NUMBER_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "cm_char.h"

// --- Squad/shirt number --- //
class PlayerSquadNumber : public CM_Char
{
public:
    PlayerSquadNumber();

    // Set data
    void set(int i);
    void set(const QVariant &value);
};

#endif // PLAYER_SQUAD_NUMBER_H
