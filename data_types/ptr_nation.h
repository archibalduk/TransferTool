#ifndef PTR_NATION_H
#define PTR_NATION_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "data_pointer.h"

// --- Club pointer --- //
class PtrNation : public DataPointer
{
public:
    PtrNation() {}
    PtrNation(const int i);

    // Counter
    void incrementClubCount();
    void incrementStaffCount();

    // Get data
    QString getSafeText() const;
    QString getText() const;

    // Set data
    void set(const int &i);
    void set(const QVariant &value);
};

#endif // PTR_NATION_H
