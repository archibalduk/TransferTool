#include "ptr_player.h"

PtrPlayer::PtrPlayer()
{

}

PtrPlayer::PtrPlayer(const int i) :
    DataPointer(i)
{

}

// --- Get pointer to underlying data --- //
Player *PtrPlayer::data()
{
    if(id < VALID)
        return nullptr;
    else if(id < Player::db.size())
        return &(Player::db[id]);
    else
        return nullptr;
}
