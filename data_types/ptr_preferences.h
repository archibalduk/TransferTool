#ifndef PTR_PREFERENCES_H
#define PTR_PREFERENCES_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "data_pointer.h"

// --- Staff preferences pointer --- //
class PtrPreferences : public DataPointer
{
public:
    PtrPreferences();
    PtrPreferences(const int i);
};

#endif // PTR_PREFERENCES_H
