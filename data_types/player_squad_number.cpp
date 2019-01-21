#include "player_squad_number.h"

PlayerSquadNumber::PlayerSquadNumber()
{
    m_Value = 0;
}

// --- Set data (int) --- //
void PlayerSquadNumber::set(int i)
{
    if(i < 1 || i > 50)
        m_Value = 0;
    else
        m_Value = static_cast<char>(i);
}

// --- Set from QVariant --- //
void PlayerSquadNumber::set(const QVariant &value)
{
    QString tmp = value.toString();

    if(tmp.isEmpty() || tmp.contains("-"))
        return;

    const char i = static_cast<char>(value.toInt());

    if(i < 1 || i > 50)
        m_Value = 0;
    else
        m_Value = static_cast<char>(i);
}
