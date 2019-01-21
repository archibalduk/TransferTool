#ifndef PTR_NON_PLAYER_H
#define PTR_NON_PLAYER_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "data_pointer.h"
#include "../database/non_player.h"

// --- Player pointer --- //
class PtrNonPlayer : public DataPointer
{
public:
    PtrNonPlayer();
    PtrNonPlayer(const int i);

    // Get data
    NonPlayer *data();
};

#endif // PTR_NON_PLAYER_H
