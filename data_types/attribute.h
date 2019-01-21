#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "cm_char.h"

// ---Player/non-player attribute (0-20 scale) --- //
class Attribute : public CM_Char
{
public:
    Attribute();

    // Set data
    void set(const QVariant &value);
};

#endif // ATTRIBUTE_H
