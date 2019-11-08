#include "staff.h"
#include <QStringList>

// --- Static data --- //
QVector<Staff> Staff::db;


/* ==================== */
/*      Staff Data      */
/* ==================== */

// --- Default constructor --- //
Staff::Staff() :
    ID(db.size())   // Allocate next ID to item
{
    // Default to a player type
    Classification.set(StaffClassification::PLAYER);
    JobForClub.set(Job::PLAYER);
    PlayingSquad.set(Squad::CLUB_SENIOR_SQUAD);
}


/* ========================= */
/*      Data Validation      */
/* ========================= */

// --- Validate data --- //
void Staff::validate()
{
    const int sz = db.size();

    for(int i = 0; i < sz; ++i) {
        // Validate date of birth
        db[i].DateOfBirth.validate();

        // Validate year of birth
        db[i].YearOfBirth.validate(db[i].DateOfBirth.year());

        // Name counters
        db[i].CommonName.increment();
        db[i].FirstName.increment();
        db[i].SecondName.increment();

        // Nation counters
        db[i].Nation.incrementStaffCount();

        // Club rosters
        db[i].ClubContracted.addToRoster(i, db[i].JobForClub);
        db[i].NationContracted.addToRoster(i, db[i].JobForNation);
    }
}


/* ===================== */
/*      Export Data      */
/* ===================== */

// --- Export staff list with extended data --- //
bool Staff::exportExtendedList(Spreadsheet &s)
{
    // Header
    QStringList header;
    header << "First name" << "Second name" << "Common name" << "DOB"
           << "Nation1" << "Nation2"
           << "Date joined club" << "Contract end"
           << "Est. Value"
           << "Club"
           << "GK" << "SW" << "D" << "DM" << "M" << "AM" << "ST" << "WB" << "FR"
           << "Right" << "Left" << "Center"
           << "Right Foot" << "Left Foot"
           << "Int Caps" << "Int Goals"
           << "Current Ability" << "Potential Ability"
           << "World Reputation" << "Home Reputation" << "Current Reputation"
           << "Squad no.";

    s.addHeader(header);

    // Temp data match text settings
    int yearAdjustment = 0;
    bool useFullDate = true;

    // Rows
    for(Staff &itr : db) {
        QStringList row;

        // Common staff data
        row << itr.FirstName.getSafeText() << itr.SecondName.getSafeText() << itr.CommonName.getSafeText()
            << itr.DateOfBirth.getMatchText(yearAdjustment, useFullDate)
            << itr.Nation.getSafeText() << itr.SecondNation.getSafeText()
            << itr.DateJoinedClub.getMatchText(yearAdjustment, useFullDate) << itr.ContractExpiresClub.getMatchText(yearAdjustment, useFullDate)
            << itr.EstimatedValue.getText()
            << itr.ClubContracted.getSafeText();

        // Player data
        Player *p = itr.PlayerData.data();

        if(p != nullptr) {
            row << p->Goalkeeper.getText() << p->Sweeper.getText() << p->Defender.getText()
                << p->DefensiveMidfielder.getText() << p->Midfielder.getText() << p->AttackingMidfielder.getText()
                << p->Attacker.getText() << p->WingBack.getText() << p->FreeRole.getText()
                << p->RightSide.getText() << p->LeftSide.getText() << p->Central.getText()
                << p->RightFoot.getText() << p->LeftFoot.getText();
        }
        else {
            row << "" << "" << "" << "" << "" << "" << "" << "" << ""
                << "" << "" << ""
                << "" << "";
        }

        // International data
        row << itr.InternationalApps.getText() << itr.InternationalGoals.getText();

        // Remaining player data
        if(p != nullptr) {
            row << p->CurrentAbility.getText() << p->PotentialAbility.getText()
                << p->WorldReputation.getText() << p->HomeReputation.getText() << p->CurrentReputation.getText()
                << p->SquadNumber.getText();
        }

        s.add(row);
    }

    return true;
}

// --- Export staff list in the form of a Transfer Importer spreadsheet --- //
bool Staff::exportTransferList(Spreadsheet &s)
{
    // Header
    QStringList header;
    header << "First name" << "Second name" << "Common name" << "DOB"
           << "Nation1" << "Nation2"
           << "Contract start" << "Contract end"
           << "Value"
           << "Club" << "On loan from"
           << "GK" << "SW" << "D" << "DM" << "M" << "AM" << "ST" << "WB" << "FR"
           << "Right" << "Left" << "Center"
           << "Right Foot" << "Left Foot"
           << "Int Caps" << "Int Goals"
           << "Current Ability" << "Potential Ability"
           << "Home Reputation" << "Current Reputation"
           << "Squad no.";

    s.addHeader(header);

    // Temp data match text settings
    int yearAdjustment = 0;
    bool useFullDate = true;

    // Rows
    for(Staff &itr : db) {
        QStringList row;

        // Common staff data
        row << itr.FirstName.getSafeText() << itr.SecondName.getSafeText() << itr.CommonName.getSafeText()
            << itr.DateOfBirth.getMatchText(yearAdjustment, useFullDate)
            << itr.Nation.getSafeText() << itr.SecondNation.getSafeText()
            << itr.DateJoinedClub.getMatchText(yearAdjustment, useFullDate) << itr.ContractExpiresClub.getMatchText(yearAdjustment, useFullDate)
            << ""
            << itr.ClubContracted.getSafeText() << "";

        // Player data
        Player *p = itr.PlayerData.data();

        if(p != nullptr) {
            row << p->Goalkeeper.getText() << p->Sweeper.getText() << p->Defender.getText()
                << p->DefensiveMidfielder.getText() << p->Midfielder.getText() << p->AttackingMidfielder.getText()
                << p->Attacker.getText() << p->WingBack.getText() << p->FreeRole.getText()
                << p->RightSide.getText() << p->LeftSide.getText() << p->Central.getText()
                << p->RightFoot.getText() << p->LeftFoot.getText();
        }
        else {
            row << "" << "" << "" << "" << "" << "" << "" << "" << ""
                << "" << "" << ""
                << "" << "";
        }

        // International data
        row << itr.InternationalApps.getText() << itr.InternationalGoals.getText();

        // Remaining player data
        if(p != nullptr) {
            row << p->CurrentAbility.getText() << p->PotentialAbility.getText()
                << p->HomeReputation.getText() << p->CurrentReputation.getText()
                << p->SquadNumber.getText();
        }

        s.add(row);
    }

    return true;
}

// --- Export staff list in the form of a Transfer Importer spreadsheet (NO ACCENTS) --- //
bool Staff::exportTransferList_NoAccents(Spreadsheet &s)
{
    // Header
    QStringList header;
    header << "First name" << "Second name" << "Common name" << "DOB"
           << "Nation1" << "Nation2"
           << "Contract start" << "Contract end"
           << "Value"
           << "Club" << "On loan from"
           << "GK" << "SW" << "D" << "DM" << "M" << "AM" << "ST" << "WB" << "FR"
           << "Right" << "Left" << "Center"
           << "Right Foot" << "Left Foot"
           << "Int Caps" << "Int Goals"
           << "Current Ability" << "Potential Ability"
           << "Home Reputation" << "Current Reputation"
           << "Squad no.";

    s.addHeader(header);

    // Temp data match text settings
    int yearAdjustment = 0;
    bool useFullDate = true;

    // Rows
    for(Staff &itr : db) {
        QStringList row;

        // Common staff data
        row << itr.FirstName.getNoAccentsText() << itr.SecondName.getNoAccentsText() << itr.CommonName.getNoAccentsText()
            << itr.DateOfBirth.getMatchText(yearAdjustment, useFullDate)
            << itr.Nation.getSafeText() << itr.SecondNation.getSafeText()
            << itr.DateJoinedClub.getMatchText(yearAdjustment, useFullDate) << itr.ContractExpiresClub.getMatchText(yearAdjustment, useFullDate)
            << ""
            << itr.ClubContracted.getNoAccentsText() << "";

        // Player data
        Player *p = itr.PlayerData.data();

        if(p != nullptr) {
            row << p->Goalkeeper.getText() << p->Sweeper.getText() << p->Defender.getText()
                << p->DefensiveMidfielder.getText() << p->Midfielder.getText() << p->AttackingMidfielder.getText()
                << p->Attacker.getText() << p->WingBack.getText() << p->FreeRole.getText()
                << p->RightSide.getText() << p->LeftSide.getText() << p->Central.getText()
                << p->RightFoot.getText() << p->LeftFoot.getText();
        }
        else {
            row << "" << "" << "" << "" << "" << "" << "" << "" << ""
                << "" << "" << ""
                << "" << "";
        }

        // International data
        row << itr.InternationalApps.getText() << itr.InternationalGoals.getText();

        // Remaining player data
        if(p != nullptr) {
            row << p->CurrentAbility.getText() << p->PotentialAbility.getText()
                << p->HomeReputation.getText() << p->CurrentReputation.getText()
                << p->SquadNumber.getText();
        }

        s.add(row);
    }

    return true;
}

// --- Export PLAYERS list in the form of a Transfer Importer spreadsheet --- //
bool Staff::exportTransferListPlayers(Spreadsheet &s)
{
    // Header
    QStringList header;
    header << "First name" << "Second name" << "Common name" << "DOB"
           << "Nation1" << "Nation2"
           << "Contract start" << "Contract end"
           << "Value"
           << "Club" << "On loan from"
           << "GK" << "SW" << "D" << "DM" << "M" << "AM" << "ST" << "WB" << "FR"
           << "Right" << "Left" << "Center"
           << "Right Foot" << "Left Foot"
           << "Int Caps" << "Int Goals"
           << "Current Ability" << "Potential Ability"
           << "Home Reputation" << "Current Reputation"
           << "Squad no.";

    s.addHeader(header);

    // Temp data match text settings
    int yearAdjustment = 0;
    bool useFullDate = true;

    // Rows
    for(Staff &itr : db) {
        if(itr.Classification.isNonPlayer())
            continue;

        QStringList row;

        // Common staff data
        row << itr.FirstName.getSafeText() << itr.SecondName.getSafeText() << itr.CommonName.getSafeText()
            << itr.DateOfBirth.getMatchText(yearAdjustment, useFullDate)
            << itr.Nation.getSafeText() << itr.SecondNation.getSafeText()
            << itr.DateJoinedClub.getMatchText(yearAdjustment, useFullDate) << itr.ContractExpiresClub.getMatchText(yearAdjustment, useFullDate)
            << ""
            << itr.ClubContracted.getSafeText() << "";

        // Player data
        Player *p = itr.PlayerData.data();

        if(p != nullptr) {
            row << p->Goalkeeper.getText() << p->Sweeper.getText() << p->Defender.getText()
                << p->DefensiveMidfielder.getText() << p->Midfielder.getText() << p->AttackingMidfielder.getText()
                << p->Attacker.getText() << p->WingBack.getText() << p->FreeRole.getText()
                << p->RightSide.getText() << p->LeftSide.getText() << p->Central.getText()
                << p->RightFoot.getText() << p->LeftFoot.getText();
        }
        else {
            row << "" << "" << "" << "" << "" << "" << "" << "" << ""
                << "" << "" << ""
                << "" << "";
        }

        // International data
        row << itr.InternationalApps.getText() << itr.InternationalGoals.getText();

        // Remaining player data
        if(p != nullptr) {
            row << p->CurrentAbility.getText() << p->PotentialAbility.getText()
                << p->HomeReputation.getText() << p->CurrentReputation.getText()
                << p->SquadNumber.getText();
        }

        s.add(row);
    }

    return true;
}

// --- Export PLAYERS list in the form of a Transfer Importer spreadsheet (NO ACCENTS) --- //
bool Staff::exportTransferListPlayers_NoAccents(Spreadsheet &s)
{
    // Header
    QStringList header;
    header << "First name" << "Second name" << "Common name" << "DOB"
           << "Nation1" << "Nation2"
           << "Contract start" << "Contract end"
           << "Value"
           << "Club" << "On loan from"
           << "GK" << "SW" << "D" << "DM" << "M" << "AM" << "ST" << "WB" << "FR"
           << "Right" << "Left" << "Center"
           << "Right Foot" << "Left Foot"
           << "Int Caps" << "Int Goals"
           << "Current Ability" << "Potential Ability"
           << "Home Reputation" << "Current Reputation"
           << "Squad no.";

    s.addHeader(header);

    // Temp data match text settings
    int yearAdjustment = 0;
    bool useFullDate = true;

    // Rows
    for(Staff &itr : db) {
        if(itr.Classification.isNonPlayer())
            continue;

        QStringList row;

        // Common staff data
        row << itr.FirstName.getNoAccentsText() << itr.SecondName.getNoAccentsText() << itr.CommonName.getNoAccentsText()
            << itr.DateOfBirth.getMatchText(yearAdjustment, useFullDate)
            << itr.Nation.getSafeText() << itr.SecondNation.getSafeText()
            << itr.DateJoinedClub.getMatchText(yearAdjustment, useFullDate) << itr.ContractExpiresClub.getMatchText(yearAdjustment, useFullDate)
            << ""
            << itr.ClubContracted.getNoAccentsText() << "";

        // Player data
        Player *p = itr.PlayerData.data();

        if(p != nullptr) {
            row << p->Goalkeeper.getText() << p->Sweeper.getText() << p->Defender.getText()
                << p->DefensiveMidfielder.getText() << p->Midfielder.getText() << p->AttackingMidfielder.getText()
                << p->Attacker.getText() << p->WingBack.getText() << p->FreeRole.getText()
                << p->RightSide.getText() << p->LeftSide.getText() << p->Central.getText()
                << p->RightFoot.getText() << p->LeftFoot.getText();
        }
        else {
            row << "" << "" << "" << "" << "" << "" << "" << "" << ""
                << "" << "" << ""
                << "" << "";
        }

        // International data
        row << itr.InternationalApps.getText() << itr.InternationalGoals.getText();

        // Remaining player data
        if(p != nullptr) {
            row << p->CurrentAbility.getText() << p->PotentialAbility.getText()
                << p->HomeReputation.getText() << p->CurrentReputation.getText()
                << p->SquadNumber.getText();
        }

        s.add(row);
    }

    return true;
}

/* ================== */
/*      Get Data      */
/* ================== */

// --- Get data --- //
QString Staff::getDisplayText()
{
    if(this->CommonName.isNone())
        return QString("%1 %2").arg(this->FirstName.getText()).arg(this->SecondName.getText());
    else
        return this->CommonName.getText();
}


/* ==================== */
/*      Match Data      */
/* ==================== */

// --- Populate a hash with staff data (with a main dob hash and a failsafe yob hash) --- //
void Staff::createHash(QHash<QString, int> &dobHash, QHash<QString, int> &yobHash, unsigned char scope,
                       int yearAdjustment, bool useFullDateOfBirth)
{
    for(Staff itr : db) {
        // Only add staff within the scope
        if( (scope & SCOPE_PLAYERS && itr.Classification.isPlayer()) ||
            (scope & SCOPE_NON_PLAYERS && itr.Classification.isNonPlayer())) {

            // Date of birth
            QString dob = itr.DateOfBirth.getMatchText(yearAdjustment, useFullDateOfBirth);
            QString yob = itr.YearOfBirth.getMatchText(yearAdjustment);

            // Add common name if available
            if(itr.CommonName.isPointer()) {
                dobHash.insert(QString("%1 %2").arg(itr.CommonName.getMatchText()).arg(dob), itr.ID);
                yobHash.insert(QString("%1 %2").arg(itr.CommonName.getMatchText()).arg(yob), itr.ID);
            }

            // Add full name if available
            if(itr.FirstName.isPointer() && itr.SecondName.isPointer()) {                
                dobHash.insert(QString("%1 %2 %3").arg(itr.FirstName.getMatchText()).arg(itr.SecondName.getMatchText()).arg(dob), itr.ID);
                yobHash.insert(QString("%1 %2 %3").arg(itr.FirstName.getMatchText()).arg(itr.SecondName.getMatchText()).arg(yob), itr.ID);
            }
        }
    }
}
