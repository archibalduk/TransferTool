#ifndef STAFF_H
#define STAFF_H

#include <QHash>
#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/attribute.h"
#include "../data_types/cm_int.h"
#include "../data_types/cm_uchar.h"
#include "../data_types/data_types.h"
#include "../data_types/date.h"
#include "../data_types/job.h"
#include "../data_types/ptr_club.h"
#include "../data_types/ptr_name.h"
#include "../data_types/ptr_nation.h"
#include "../data_types/ptr_non_player.h"
#include "../data_types/ptr_player.h"
#include "../data_types/ptr_preferences.h"
#include "../data_types/squad.h"
#include "../data_types/staff_classification.h"
#include "../data_types/year.h"
#include "../data_types/wage.h"
#include "../spreadsheet/spreadsheet.h"

// Forward declaration for purposes of friend class
class TransferImporter;

// --- Staff.dat - Staff --- //
class Staff
{   
private:
    // Database data
    int ID;
    PtrFirstName FirstName;
    PtrSecondName SecondName;
    PtrCommonName CommonName;
    Date DateOfBirth;
    Year YearOfBirth;
    PtrNation Nation;
    PtrNation SecondNation;
    CM_UChar InternationalApps;
    CM_UChar InternationalGoals;
    PtrClub NationContracted;
    Job JobForNation;
    Date DateJoinedNation;
    Date ContractExpiresNation;
    PtrClub ClubContracted;
    Job JobForClub;
    Date DateJoinedClub;
    Date ContractExpiresClub;
    Wage EstimatedWage;
    CM_Int EstimatedValue;
    Attribute Adaptability;
    Attribute Ambition;
    Attribute Determination;
    Attribute Loyalty;
    Attribute Pressure;
    Attribute Professionalism;
    Attribute Sportsmanship;
    Attribute Temperament;
    Squad PlayingSquad;
    StaffClassification Classification;
    CM_Char ClubValuation; // This is possibly a 0-20 range
    PtrPlayer PlayerData;
    PtrPreferences Preferences; // Version 0x02 - New ptr type
    PtrNonPlayer NonPlayerData;

    // Runtime data
    char EuroSquadFlag;

public:
    // Constructor
    Staff();

    // Container
    static QVector<Staff> db;

    // Data validation
    static void validate();

    // Export data
    static bool exportExtendedList(Spreadsheet &s);
    static bool exportTransferList(Spreadsheet &s);
    static bool exportTransferList_NoAccents(Spreadsheet &s);
    static bool exportTransferListPlayers_NoAccents(Spreadsheet &s);

    // Get data
    QString getDisplayText();

    // Match data
    static void createHash(QHash<QString, int> &dobHash, QHash<QString, int> &yobHash, unsigned char scope,
                           int yearAdjustment = 0, bool useFullDateOfBirth = true);

    enum ENUM_MATCH_SCOPE {
        SCOPE_NON_PLAYERS = 1,
        SCOPE_PLAYERS = SCOPE_NON_PLAYERS * 2   // Use powers of two for effective bitwise operators
    };

    // Friend class
    friend class TransferImporter;
};

#endif // STAFF_H
