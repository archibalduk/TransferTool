#ifndef CLUB_H
#define CLUB_H

#include <QHash>
#include <QString>
#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "../data_types/string.h"
#include "../data_types/data_types.h"
#include "../data_types/job.h"
#include "../data_types/ptr_club.h"
#include "../data_types/ptr_club_comp.h"
#include "../data_types/ptr_nation.h"
#include "../data_types/ptr_staff.h"
#include "../spreadsheet/spreadsheet.h"

// --- Club.dat --- //
class Club
{
private:
    // Database data
    int ID;
    char Name[ STANDARD_TEXT_LENGTH ];
    char GenderName;
    char NameShort[ SHORT_TEXT_LENGTH ];
    char GenderNameShort;
    PtrNation Nation;
    PtrClubComp Division;
    PtrClubComp LastDivision;
    char LastPosition;
    PtrClubComp ReserveDivision;
    char ProfessionalStatus;
    int Cash;
    int Stadium;
    char OwnStadium;
    int ReserveStadium;
    char HomeMatchDay;
    int Attendance;
    int MinAttendance;
    int MaxAttendance;
    char Training;
    short Reputation; // Version 0x02 - Changed char->short
    char PLC;
    int ForegroundColour1;
    int BackgroundColour1;
    int ForegroundColour2;
    int BackgroundColour2;
    int ForegroundColour3;
    int BackgroundColour3;
    PtrStaff FavouriteStaff1;
    PtrStaff FavouriteStaff2;
    PtrStaff FavouriteStaff3;
    PtrStaff DislikedStaff1;
    PtrStaff DislikedStaff2;
    PtrStaff DislikedStaff3;
    PtrClub Rivals1;
    PtrClub Rivals2;
    PtrClub Rivals3;
    PtrStaff Chairman;
    PtrStaff DirectorList[ DIRECTOR_SIZE ];
    PtrStaff Manager;
    PtrStaff AssistantManager;
    PtrStaff SquadList[ SQUAD_SIZE ];
    PtrStaff CoachList[ COACH_SIZE ];
    PtrStaff ScoutList[ SCOUT_SIZE ];
    PtrStaff PhysioList[ PHYSIO_SIZE ];

    // Runtime data
    int EuroFlag;
    char EuroSeeding;
    int TeamSelected[ TEAM_SZ ];
    int TacticTraining[ MAX_TACTIC_TRAINING ];
    int TacticSelected;
    char HasLinkedClub;

public:
    // Constructor
    Club();

    // Containers (domestic & international)
    static QVector<Club> dbDom;
    static QVector<Club> dbInt;

    // Data validation
    static void validate();

    // Export data
    static bool exportData(Spreadsheet &s);
    static bool exportRosters(Spreadsheet &s);
    void exportRow(QStringList &row);

    // Get data
    QString getLongName();
    QString getShortName();

    // Match data
    static void createHash(QHash<QString, int> &hash, bool useLongNames = true);

    // Rosters
    bool addToRoster(const PtrStaff &staff, const Job &job);
    static void clearRosters();
};

#endif // CLUB_H
