#include "player.h"

// --- Static data --- //
QVector<Player> Player::db;


/* ===================== */
/*      Player Data      */
/* ===================== */

// --- Default constructor --- //
Player::Player() :
    ID(db.size())   // Allocate next ID to item
{
    // Default squad number
    SquadNumber.set(0);

    // Default ability
    CurrentAbility.set(0);
    PotentialAbility.set(0);

    // Default positional ratings
    Goalkeeper.set(1);
    Sweeper.set(1);
    Defender.set(1);
    DefensiveMidfielder.set(1);
    Midfielder.set(1);
    AttackingMidfielder.set(1);
    Attacker.set(1);
    WingBack.set(1);
}


/* ================== */
/*      Add Data      */
/* ================== */

// --- Add new entry --- //
int Player::addNewItem()
{
    Player tmp;
    db.push_back(tmp);
    return tmp.ID;
}
