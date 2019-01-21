#ifndef PTR_CLUB_H
#define PTR_CLUB_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "data_pointer.h"
#include "job.h"

// --- Club pointer --- //
class PtrClub : public DataPointer
{
public:
    // Constructor
    PtrClub();
    PtrClub(const int i);

    // Get data
    QString getNoAccentsText() const;
    QString getSafeText() const;
    QString getText() const;

    // Set data
    bool addToRoster(const int staffId, const Job &job);
    void set(const int &i);
    void set(const QVariant &value);
};

#endif // PTR_CLUB_H
