#ifndef PLAYER_H
#define PLAYER_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/attribute.h"
#include "../data_types/ability.h"
#include "../data_types/data_types.h"
#include "../data_types/player_squad_number.h"
#include "../data_types/reputation.h"

// Forward declaration for purposes of friend class
class Staff;
class TransferImporter;

// --- Staff.dat - Player attributes --- //
class Player
{
private:
    // Database data
    int ID;
    PlayerSquadNumber SquadNumber;
    AbilityCurrent CurrentAbility;
    AbilityPotential PotentialAbility;
    Reputation HomeReputation; // Version 0x02 - Changed char->short
    Reputation CurrentReputation; // Version 0x02 - Changed char->short
    Reputation WorldReputation; // Version 0x02 - Changed char->short
    Attribute Goalkeeper;
    Attribute Sweeper;
    Attribute Defender;
    Attribute DefensiveMidfielder;
    Attribute Midfielder;
    Attribute AttackingMidfielder;
    Attribute Attacker;
    Attribute WingBack;
    Attribute RightSide;
    Attribute LeftSide;
    Attribute Central;
    Attribute FreeRole;
    Attribute Acceleration;
    Attribute Aggression;
    Attribute Agility;
    Attribute Anticipation;
    Attribute Balance;
    Attribute Bravery; // Value
    Attribute Consistency;
    Attribute Corners;
    Attribute Crossing;
    Attribute Decisions;
    Attribute Dirtiness;
    Attribute Dribbling;
    Attribute Finishing;
    Attribute Flair;
    Attribute FreeKicks;
    Attribute Handling;
    Attribute Heading;
    Attribute ImportantMatches;
    Attribute InjuryProneness;
    Attribute Jumping;
    Attribute Leadership;
    Attribute LeftFoot;
    Attribute LongShots;
    Attribute Marking;
    Attribute Movement;
    Attribute NaturalFitness;
    Attribute OneOnOnes;
    Attribute Pace;
    Attribute Passing;
    Attribute Penalties;
    Attribute Positioning;
    Attribute Reflexes;
    Attribute RightFoot;
    Attribute Stamina;
    Attribute Strength;
    Attribute Tackling;
    Attribute Teamwork;
    Attribute Technique;
    Attribute ThrowIns;
    Attribute Versatility;
    Attribute Vision;
    Attribute WorkRate;

    // runtime data
    Attribute Morale;

public:
    // Constructor
    Player();

    // Container
    static QVector<Player> db;

    // Friend class
    friend class Staff;
    friend class TransferImporter;

    // Add data
    static int addNewItem();
};

#endif // PLAYER_H
