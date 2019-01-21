#ifndef OFFICIAL_H
#define OFFICIAL_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/data_types.h"
#include "../data_types/date.h"
#include "../data_types/ptr_name.h"
#include "../data_types/ptr_nation.h"
#include "../data_types/year.h"

// --- Colour.dat --- //
class Official
{
private:
    int ID;
    PtrFirstName FirstName;
    PtrSecondName SecondName;
    Date DateOfBirth;
    Year YearOfBirth;
    PtrNation Nation;
    int City;
    short CurrentAbility;
    short PotentialAbility;
    short Reputation;
    char AllowingFlow;
    char Discipline;
    char ImportantMatches;
    char Pressure;
    char Refereeing;
    char RunningLine;
    char Timekeeping;

public:
    // Constructor
    Official();

    // Container
    static QVector<Official> db;

    // Data validation
    static void validate();
};

#endif // OFFICIAL_H
