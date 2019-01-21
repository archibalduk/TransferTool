#include "cm_uchar.h"
#include <QString>

// --- Default constructor --- //
CM_UChar::CM_UChar() :
    m_Value(0)
{

}

// --- Get data (short) --- //
unsigned short CM_UChar::get() const
{
    return static_cast<unsigned short>(m_Value);
}

// --- Get data (QString) --- //
QString CM_UChar::getText() const
{
    return QString::number(m_Value);
}


// --- Set --- //
void CM_UChar::set(const int &i)
{
    m_Value = static_cast<unsigned char>(i);
}

// --- Set --- //
void CM_UChar::set(const QVariant &value)
{
    QString tmp = value.toString();

    if(tmp.isEmpty() || tmp.contains("-"))
        return;
    else
        m_Value = static_cast<unsigned char>(value.toInt());
}
