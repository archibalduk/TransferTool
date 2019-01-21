#include "cm_int.h"

CM_Int::CM_Int(const int &i) :
    m_Value(i)
{

}

// --- Get data --- //
int CM_Int::get() const
{
    return m_Value;
}

// --- Get data as text --- //
QString CM_Int::getText() const
{
    return QString::number(m_Value);
}

// --- Set --- //
void CM_Int::set(const int &i)
{
    m_Value = i;
}

// --- Set --- //
void CM_Int::set(const QVariant &value, double exchangeRate)
{
    const QString tmp = value.toString();

    if(tmp.isEmpty() || tmp.contains("-"))
        return;
    else
        m_Value = static_cast<int>(value.toFloat() * exchangeRate);
}
