#ifndef WAGE_H
#define WAGE_H

#pragma warning(disable: 4103)
#pragma pack(1)

#include "cm_int.h"

// --- Wage --- //
class Wage : public CM_Int
{
public:
    Wage();
    void setFromWorldReputation(const short &rep);
};

#endif // WAGE_H
