#ifndef SQUAD_H
#define SQUAD_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "cm_char.h"

// --- Playing Squad --- //
class Squad : public CM_Char
{
public:
    Squad();

    // Set data
    void set(int i);

    // Values
    enum ENUM_SQUAD {
        INVALID_SQUAD = 0,
        CLUB_SENIOR_SQUAD = 0x01,
        CLUB_RESERVE_SQUAD = 0x02,
        NATION_MAIN_SQUAD = 0x04,
        NATION_B_SQUAD = 0x08
    };
};

#endif // SQUAD_H
