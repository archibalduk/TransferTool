#ifndef PTR_PLAYER_H
#define PTR_PLAYER_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "data_pointer.h"
#include "../database/player.h"

// --- Player pointer --- //
class PtrPlayer : public DataPointer
{
public:
    PtrPlayer();
    PtrPlayer(const int i);

    // Get data
    Player *data();
};

#endif // PTR_PLAYER_H
