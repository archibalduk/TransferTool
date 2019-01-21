#include "cm_short.h"

CM_Short::CM_Short(const short &i) :
    m_Value(i)
{

}

// --- Get data --- //
short CM_Short::get() const
{
    return m_Value;
}

// --- Get data (QString) --- //
QString CM_Short::getText() const
{
    return QString::number(m_Value);
}

// --- Set data (int) --- //
void CM_Short::set(int &i)
{
    m_Value = static_cast<short>(i);
}

// --- Set data (short) --- //
void CM_Short::set(short &i)
{
    m_Value = i;
}
