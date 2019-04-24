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
    qint32 IDR;
    qint32 FirstNameId;
    qint32 SecondNameId;
    qint32 CommonNameId;
    Date DateOfBirthR;
    qint16 YearOfBirthR;
    qint32 NationR;
    qint32 SecondNationR;
    quint8 InternationalAppsR;
    quint8 InternationalGoalsR;
    qint32 NationContractedR;
    qint8 JobForNationR;
    Date DateJoinedNationR;
    Date ContractExpiresNationR;
    qint32 ClubContractedR;
    qint8 JobForClubR;
    Date DateJoinedClubR;
    Date ContractExpiresClubR;
    qint32 EstimatedWageR;
    qint32 EstimatedValueR;
    qint8 AdaptabilityR;
    qint8 AmbitionR;
    qint8 DeterminationR;
    qint8 LoyaltyR;
    qint8 PressureR;
    qint8 ProfessionalismR;
    qint8 SportsmanshipR;
    qint8 TemperamentR;
    qint8 PlayingSquadR;
    qint8 ClassificationR;
    qint8 ClubValuationR;
    qint32 PlayerDataR;
    qint32 PreferencesR; // Version 0x02 - New ptr type
    qint32 NonPlayerDataR;

    // Runtime data
    qint8 EuroSquadFlagR;

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

    // File I/O
    void read(QDataStream &in);

public:
    // Constructor
    Staff();    

    // File I/O
    static qint32 readAll(QDataStream &in, const qint32 &count);

    // Container
    static QVector<Staff> db;

    void fillFromDatabase(int sr);

    // Data validation
    static void validate();

    // Export data
    static bool exportExtendedList(Spreadsheet &s);
    static bool exportTransferList(Spreadsheet &s);
    static bool exportTransferList_NoAccents(Spreadsheet &s);

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
