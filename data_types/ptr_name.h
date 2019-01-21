#ifndef PTR_NAME_H
#define PTR_NAME_H

#pragma warning(disable: 4103)
#pragma pack(1)

// Base class
#include "data_pointer.h"

// --- Common Name pointer --- //
class PtrCommonName : public DataPointer
{
public:
    // Constructor
    PtrCommonName();

    // Counter
    void increment();

    // Get data
    QString getMatchText() const;
    QString getNoAccentsText() const;
    QString getSafeText() const;
    QString getText() const;
    bool isNone() const;

    // Set data
    void set(const int &i);
    void set(const QVariant &value);
    void set(const QString &name, const int &nation);
};

// --- First Name pointer --- //
class PtrFirstName : public DataPointer
{
public:
    // Constructor
    PtrFirstName();

    // Counter
    void increment();

    // Get data
    QString getMatchText() const;
    QString getNoAccentsText() const;
    QString getSafeText() const;
    QString getText() const;

    // Set data
    void set(const int &i);
    void set(const QVariant &value);
    void set(const QString &name, const int &nation);
};

// --- Second Name pointer --- //
class PtrSecondName : public DataPointer
{
public:
    // Constructor
    PtrSecondName();

    // Counter
    void increment();

    // Get data
    QString getMatchText() const;
    QString getNoAccentsText() const;
    QString getSafeText() const;
    QString getText() const;

    // Set data
    void set(const int &i);
    void set(const QVariant &value);
    void set(const QString &name, const int &nation);
};

#endif // PTR_NAME_H
