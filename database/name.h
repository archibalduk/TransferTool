#ifndef NAME_H
#define NAME_H

#include <QString>
#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/string.h"
#include "../data_types/data_types.h"
#include "../data_types/ptr_nation.h"


// --- Name base class --- //
class Name
{
protected:
    char      NameText[ STANDARD_TEXT_LENGTH ];
    int       ID;
    PtrNation Nation;
    char      Count;

    // Additional data
    QString     StringText; // QString representation of the raw NameText char array

public:
    // Constructor
    Name();

    // Counters
    void decrementCount();
    void incrementCount();
    void resetCount();

    // Get data
    QString getName();
};

// --- Common_names.dat --- //
class CommonName : public Name
{
public:
    // Constructor
    CommonName();

    // Containers
    static QVector<CommonName> db;

    // Counters
    static void resetCounters();

    // Set data
    static int set(const QString &name, const int &nation);
    static void setStringText();
};

// --- First_names.dat --- //
class FirstName : public Name
{
public:
    // Constructor
    FirstName();

    // Containers
    static QVector<FirstName> db;

    // Counters
    static void resetCounters();

    // Set data
    static int set(const QString &name, const int &nation);
    static void setStringText();
};

// --- Second_names.dat --- //
class SecondName : public Name
{
public:
    // Constructor
    SecondName();

    // Containers
    static QVector<SecondName> db;

    // Counters
    static void resetCounters();

    // Set data
    static int set(const QString &name, const int &nation);
    static void setStringText();
};

#endif // NAME_H
