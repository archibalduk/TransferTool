#ifndef PTR_STAFF_H
#define PTR_STAFF_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "data_pointer.h"

// --- Staff pointer --- //
class PtrStaff : public DataPointer
{
public:
    PtrStaff();
    PtrStaff(const int i);

    // Get data
    QString getText() const;

    // Set data
    void set(const int &i);
    void set(const QVariant &value);
};

#endif // PTR_STAFF_H
