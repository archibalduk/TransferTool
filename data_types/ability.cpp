#include "ability.h"

// --- Default constructor: Base class -- //
Ability::Ability() :
    CM_Short(0)
{

}

// --- Default constructor: Current ability -- //
AbilityCurrent::AbilityCurrent() :
    Ability()
{

}

// --- Default constructor: Potential ability -- //
AbilityPotential::AbilityPotential() :
    Ability()
{

}

// --- Set from QVariant --- //
void Ability::set(const QVariant &value)
{
    QString tmp = value.toString();

    if(tmp.isEmpty())
        return;

    short i = static_cast<short>(value.toInt());

    if(i < 0)
        i = 0;
    else if(i > 200)
        i = 200;

    m_Value = i;
}

// --- Set from QVariant --- //
void AbilityPotential::set(const QVariant &value)
{
    QString tmp = value.toString();

    if(tmp.isEmpty())
        return;

    short i = static_cast<short>(value.toInt());

    if(i < -2)
        i = -2;
    else if(i > 200)
        i = 200;

    m_Value = i;
}
