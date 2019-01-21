#ifndef PTR_CLUB_COMP_H
#define PTR_CLUB_COMP_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "data_pointer.h"

// --- Club Comp pointer --- //
class PtrClubComp : public DataPointer
{
public:
    // Get data
    QString getText() const;

    // Set data
    void set(const int &i);
    void set(const QVariant &value);
};

#endif // PTR_CLUB_COMP_H
