#ifndef JOB_H
#define JOB_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "cm_char.h"

// --- Job for club/nation --- //
class Job : public CM_Char
{
public:
    Job();

    // Set data
    void set(int i);

    // Values
    enum ENUM_JOB {
        INVALID_JOB,
        CHAIRMAN,
        MANAGING_DIRECTOR,
        GENERAL_MANAGER,
        DIRECTOR_OF_FOOTBALL,
        MANAGER,
        ASSISTANT_MANAGER,
        RESERVE_TEAM_MANAGER,
        COACH,
        SCOUT,
        PHYSIO,
        PLAYER,
        PLAYER_MANAGER,
        PLAYER_ASSISTANT_MANAGER,
        PLAYER_RESERVE_TEAM_MANAGER,
        PLAYER_COACH,
        PLAYER_RETIRED,
        MEDIA_PUNDIT,
    };
};

#endif // JOB_H
