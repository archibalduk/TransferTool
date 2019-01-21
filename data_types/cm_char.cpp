#include "cm_char.h"

// --- Default constructor --- //
CM_Char::CM_Char() :
    m_Value(-1)
{

}

// --- Get data (short) --- //
short CM_Char::get() const
{
    return static_cast<short>(m_Value);
}

// --- Get data (QString) --- //
QString CM_Char::getText() const
{
    return QString::number(m_Value);
}

// --- Set data (int) --- //
void CM_Char::set(int i)
{
    m_Value = static_cast<int>(i);
}
