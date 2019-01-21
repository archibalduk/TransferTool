#include "ptr_non_player.h"

PtrNonPlayer::PtrNonPlayer()
{

}

PtrNonPlayer::PtrNonPlayer(const int i) :
    DataPointer(i)
{

}

// --- Get pointer to underlying data --- //
NonPlayer *PtrNonPlayer::data()
{
    if(id < VALID)
        return nullptr;
    else if(id < NonPlayer::db.size())
        return &(NonPlayer::db[id]);
    else
        return nullptr;
}
