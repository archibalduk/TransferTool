#ifndef NON_PLAYER_H
#define NON_PLAYER_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/attribute.h"
#include "../data_types/data_types.h"

// Forward declaration for purposes of friend class
class Staff;

// --- Staff.dat - Non-player attributes --- //
class NonPlayer
{
private:
    int ID;
    short CurrentAbility;
    short PotentialAbility;
    short HomeReputation; // Version 0x02 - Changed char->short
    short CurrentReputation; // Version 0x02 - Changed char->short
    short WorldReputation; // Version 0x02 - Changed char->short
    Attribute Attacking;
    Attribute Business;
    Attribute Coaching;
    Attribute CoachingGks;
    Attribute CoachingTechnique;
    Attribute Directness;
    Attribute Discipline;
    Attribute FreeRoles;
    Attribute Interference;
    Attribute Judgement;
    Attribute JudgingPotential;
    Attribute ManHandling;
    Attribute Marking;
    Attribute Motivating;
    Attribute Offside;
    Attribute Patience;
    Attribute Physiotherapy;
    Attribute Pressing;
    Attribute Resources;
    Attribute Tactics;
    Attribute Youngsters;
    int Goalkeeper;
    int Sweeper;
    int Defender;
    int DefensiveMidfielder;
    int Midfielder;
    int AttackingMidfielder;
    int Attacker;
    int WingBack;
    char FormationPreferred;

public:
    // Constructor
    NonPlayer();

    // Container
    static QVector<NonPlayer> db;

    // Friend class
    friend class Staff;
};

#endif // NON_PLAYER_H
