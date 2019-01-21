#ifndef NATION_H
#define NATION_H

#include <QHash>
#include <QString>
#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/data_types.h"

// --- Nation.dat --- //
class Nation
{
private:
    // Database data
    int ID;
    char Name[ STANDARD_TEXT_LENGTH ];
    char GenderName;
    char NameShort[ SHORT_TEXT_LENGTH ];
    char GenderNameShort;
    char NameThreeLetter[ 4 ];
    char NameNationality[ SHORT_TEXT_LENGTH ];
    int Continent;
    char Region;
    char ActualRegion;
    char FirstLanguage;
    char SecondLanguage;
    char ThirdLanguage;
    int CapitalCity;
    char StateOfDevelopment;
    char GroupMembership;
    int NationalStadium;
    char GameImportance;
    char LeagueStandard;
    short NumberClubs;
    int NumberStaff; // Version 0x02 - Added
    short SeasonUpdateDay; // Version 0x02 - Added
    short Reputation; // Version 0x02 - Changed char->short
    int ForegroundColour1;
    int BackgroundColour1;
    int ForegroundColour2;
    int BackgroundColour2;
    int ForegroundColour3;
    int BackgroundColour3;
    double FIFACoefficient;
    double FIFACoefficient91;
    double FIFACoefficient92;
    double FIFACoefficient93;
    double FIFACoefficient94;
    double FIFACoefficient95;
    double FIFACoefficient96;
    double UEFACoefficient91;
    double UEFACoefficient92;
    double UEFACoefficient93;
    double UEFACoefficient94;
    double UEFACoefficient95;
    double UEFACoefficient96;
    int Rivals1;
    int Rivals2;
    int Rivals3;

    // Runtime data
    char LeagueSelected;
    int ShortlistOffset; // Version 0x02 - Added
    char GamesPlayed; // Version 0x02 - Moved to runtime

public:
    // Constructor
    Nation();

    // Container
    static QVector<Nation> db;

    // Club counter
    void decrementClubCount();
    void incrementClubCount();
    void resetClubCount();

    // Staff counter
    void decrementStaffCount();
    void incrementStaffCount();
    void resetStaffCount();

    // Counters
    static void resetCounters();

    // Get data
    QString getLongName();
    QString getShortName();

    // Match data
    static void createHash(QHash<QString, int> &hash, bool useLongNames = true);
};

#endif // NATION_H
