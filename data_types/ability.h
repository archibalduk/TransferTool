#ifndef ABILITY_H
#define ABILITY_H

#pragma warning(disable: 4103)
#pragma pack(1)

#include "cm_short.h"

// --- Current/Potential Ability Base Class --- //
class Ability : public CM_Short
{
public:
    // Constructor
    Ability();

    // Set data
    void set(const QVariant &value);
};

// --- Current Ability Wrapper --- //
class AbilityCurrent : public Ability
{
public:
    // Constructor
    AbilityCurrent();
};

// --- Potential Ability Wrapper --- //
class AbilityPotential : public Ability
{
public:
    // Constructor
    AbilityPotential();

    // Set data
    void set(const QVariant &value);
};

#endif // ABILITY_H
