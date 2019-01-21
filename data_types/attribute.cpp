#include "attribute.h"
#include <QString>

Attribute::Attribute()
{
    m_Value = 0;
}

// --- Set from QVariant --- //
void Attribute::set(const QVariant &value)
{
    QString tmp = value.toString();

    if(tmp.isEmpty() || tmp.contains("-"))
        return;

    int i = value.toInt();

    if(i < 0)
        i = 0;
    else if(i > 20)
        i = 20;

    m_Value = i;
}
