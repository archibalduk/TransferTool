#include "transfer_importer.h"
#include "vlookup_hash.h"
#include "../database/club.h"
#include "../database/club_comp.h"
#include "../database/nation.h"
#include "../database/staff.h"
#include "../database/staff_preferences.h"

// Qt headers
#include <algorithm>
#include <QApplication>
#include <QGridLayout>
#include <QHash>
#include <QVBoxLayout>

// --- Static data --- //
QString TransferImporter::s_ProtectedPlayersFileName = QStringLiteral("vlookup_protected_players.csv");


/* =========================== */
/*      Transfer Importer      */
/* =========================== */

// --- Default constructor --- //
TransferImporter::TransferImporter(Settings &settings,
                                   int posSettings,
                                   int posImportPath,
                                   QWidget *parent) :
    m_ColumnAdjustment(0),
    m_Settings(&settings),
    m_SettingsPos(posSettings),
    m_SettingImportPathPos(posImportPath),
    m_Spreadsheet(nullptr),
    QWidget(parent)
{
    // Create user interface
    this->createControlWidget();
    this->createProgressWidget();
    this->createStatusWidget();
    this->createSettingsWidget();

    // Main layout
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_ControlWidget, 0, 0);
    layout->addWidget(m_ProgressWidget, 1, 0);
    layout->addWidget(m_StatusWidget, 2, 0);
    layout->addWidget(m_SettingsWidget, 0, 1, 3, 1);
    this->setLayout(layout);
}

/* ======================== */
/*      Control Widget      */
/* ======================== */

// --- Create control widget --- //
void TransferImporter::createControlWidget()
{
    // Widget & layout
    m_ControlWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(m_ControlWidget);

    // Open button
    m_OpenButton = new QPushButton("Open File", m_ControlWidget);
    QObject::connect(m_OpenButton, &QPushButton::clicked,
                     this, &TransferImporter::onOpen);

    // Save button
    m_SaveButton = new QPushButton("Save File", m_ControlWidget);
    m_SaveButton->setEnabled(false);   // Initially disabled as no file will be open to start with
    QObject::connect(m_SaveButton, &QPushButton::clicked,
                     this, &TransferImporter::onSave);

    // Import button
    m_ImportButton = new QPushButton("Import Data", m_ControlWidget);
    m_ImportButton->setEnabled(false);   // Initially disabled as no file will be open to start with
    QObject::connect(m_ImportButton, &QPushButton::clicked,
                     this, &TransferImporter::onImport);

    // Close button
    m_CloseButton = new QPushButton("Close File", m_ControlWidget);
    m_CloseButton->setEnabled(false);   // Initially disabled as no file will be open to start with
    QObject::connect(m_CloseButton, &QPushButton::clicked,
                     this, &TransferImporter::onClose);


    // Apply layout
    layout->addWidget(m_OpenButton, 0, 0);
    layout->addWidget(m_SaveButton, 0, 1);
    layout->addWidget(m_ImportButton, 0, 2);
    layout->addWidget(m_CloseButton, 0, 3);
    m_ControlWidget->setLayout(layout);
}


/* ============== */
/*      Data      */
/* ============== */

// --- Import the spreadsheet data --- //
void TransferImporter::onImport()
{
    // Temporarily disable the relevant buttons
    m_CloseButton->setEnabled(false);
    m_ImportButton->setEnabled(false);
    m_SaveButton->setEnabled(false);
    m_OpenButton->setEnabled(false);

    // Progress bar
    this->resetProgressBar();
    m_ProgressBar->setRange(0, m_Spreadsheet->data()->size());
    m_ProgressLabel->setText("Importing Data");

    // Start timer
    QTime timer;
    timer.start();

    // Process the rows of data (loop by index for optimum speed)
    const unsigned int rowSize = static_cast<unsigned int>(m_Spreadsheet->data()->size());
    for(unsigned int row = 0; row < rowSize; ++row) {

        /* DISPLAY NAME IN PROGRESS DIALOG
         * This is useful for debugging but is VERY VERY slow!!

        // Display name
        QString displayName;

        switch(m_ColumnAdjustment)
        {
        case 2:
            displayName = QString("%1 %2 %3")
                    .arg(m_Spreadsheet->string(row, NAME))
                    .arg(m_Spreadsheet->string(row, NAME+1))
                    .arg(m_Spreadsheet->string(row, NAME+2));
            break;

        case 1:
            displayName = QString("%1 %2")
                    .arg(m_Spreadsheet->string(row, NAME))
                    .arg(m_Spreadsheet->string(row, NAME+1));
                    break;
        default:
            displayName = m_Spreadsheet->string(row, NAME);
            break;
        }

        // Progress bar
        this->incrementProgressBar(displayName);

        */

        // Progress bar
        this->incrementProgressBar();    // Comment out this line if the above Display Name is being used

        if(m_IsPlayerSheet){
            // Skip any rows with a blank DOB column
            if(m_Spreadsheet->string(row, DATE_OF_BIRTH).isEmpty())
                continue;

            // Columns positions
            const unsigned int posFurthestRightColumn = m_Spreadsheet->data()->at(row).size()-3;  // Furthest right column
            const unsigned int posIsProtected = posFurthestRightColumn;
            const unsigned int posLoanClubId = posFurthestRightColumn -1;
            const unsigned int posClubId = posFurthestRightColumn - 2;
            const unsigned int posNationSecondaryId = posFurthestRightColumn - 3;
            const unsigned int posNationPrimaryId = posFurthestRightColumn - 4;
            const unsigned int posStaffId = posFurthestRightColumn - 5;

            // Id numbers
            int staffId = m_Spreadsheet->number(row, posStaffId);

            // Skip any invalid staff Ids (there should not be any)
            if(staffId == Spreadsheet::NOT_A_NUMBER) {
                continue;
            }

            // Determine relevant club ID to use according to whether the player is on loan
            QVariant club = m_Spreadsheet->variant(row, posClubId);
            QVariant loanClub = m_Spreadsheet->variant(row, posLoanClubId);

            if(loanClub.toInt() >= VALID)   // Use the loan club if the column is not blank
                club = loanClub;

            // Determine whether the player is protected
            const bool isProtected = m_Spreadsheet->variant(row, posIsProtected).toBool();

            // For flagging whether new staff to be added
            bool isNewStaff = (staffId < VALID);

            /* Add new staff */
            if(isNewStaff) {
                // Staff data
                Staff s;
                staffId = s.ID;

                // Date & year of birth
                s.DateOfBirth.set(m_Spreadsheet->matchDateString(row, DATE_OF_BIRTH), m_YearAdjustment->value());
                s.YearOfBirth.validate(s.DateOfBirth.year());

                // Player data
                Player p;
                s.PlayerData = p.ID;

                // Staff preferences data
                StaffPreferences sp;
                s.Preferences = sp.ID;

                // Add to database
                Player::db.push_back(p);
                StaffPreferences::db.push_back(sp);
                Staff::db.push_back(s);
            }

            /* Edit new and existing staff */
            // Staff pointer
            Staff *staff = &(Staff::db[staffId]);

            // Nationality
            staff->Nation.setNoCheck(m_Spreadsheet->variant(row, posNationPrimaryId));
            staff->SecondNation.setNoCheck(m_Spreadsheet->variant(row, posNationSecondaryId));

            // Contract
            staff->ClubContracted.setNoCheck(club);
            staff->DateJoinedClub.set(m_Spreadsheet->variant(row, CONTRACT_START), m_YearAdjustment->value());
            staff->ContractExpiresClub.set(m_Spreadsheet->variant(row, CONTRACT_END), m_YearAdjustment->value());

            // International stats
            staff->InternationalApps.set(m_Spreadsheet->variant(row, INT_CAPS));
            staff->InternationalGoals.set(m_Spreadsheet->variant(row, INT_GOALS));

            // Value
            staff->EstimatedValue.set(m_Spreadsheet->variant(row, VALUE), m_ExchangeRate->value());

            // Attributes
            Player *player = staff->PlayerData.data();

            if(player != nullptr) {

                // Protected player items
                if(!isProtected)
                {
                    // Positions
                    player->Goalkeeper.set(m_Spreadsheet->variant(row, GK));
                    player->Sweeper.set(m_Spreadsheet->variant(row, SW));
                    player->Defender.set(m_Spreadsheet->variant(row, D));
                    player->DefensiveMidfielder.set(m_Spreadsheet->variant(row, DM));
                    player->Midfielder.set(m_Spreadsheet->variant(row, M));
                    player->AttackingMidfielder.set(m_Spreadsheet->variant(row, AM));
                    player->Attacker.set(m_Spreadsheet->variant(row, ST));
                    player->WingBack.set(m_Spreadsheet->variant(row, WB));
                    player->FreeRole.set(m_Spreadsheet->variant(row, FR));

                    // Sides
                    player->RightSide.set(m_Spreadsheet->variant(row, SIDE_RIGHT));
                    player->LeftSide.set(m_Spreadsheet->variant(row, SIDE_LEFT));
                    player->Central.set(m_Spreadsheet->variant(row, SIDE_CENTRE));

                    // Footedness
                    player->RightFoot.set(m_Spreadsheet->variant(row, FOOT_RIGHT));
                    player->LeftFoot.set(m_Spreadsheet->variant(row, FOOT_LEFT));

                    // Ability
                    player->CurrentAbility.set(m_Spreadsheet->variant(row, ABILITY_CURRENT));
                    player->PotentialAbility.set(m_Spreadsheet->variant(row, ABILITY_POTENTIAL));

                    // Reputation
                    player->HomeReputation.set(m_Spreadsheet->variant(row, REPUTATION_HOME));
                    player->CurrentReputation.set(m_Spreadsheet->variant(row, REPUTATION_CURRENT));
                }

                // Unprotected player items:

                // World Reputation
                bool resultWR = player->WorldReputation.setFromTransferValue(m_Spreadsheet->variant(row, VALUE));

                // Wage (only if World Rep was successfully updated)
                if(resultWR == true)
                    staff->EstimatedWage.setFromWorldReputation(player->WorldReputation.get());

                // Squad number
                player->SquadNumber.set(m_Spreadsheet->variant(row, SQUAD_NUMBER));
            }

            /* Setting name of new staff */
            /* This can only be done once nationality has been set */

            /* First name in first column, second name in second column and common name in third column */
            if(isNewStaff && m_ColumnAdjustment == 2)
            {
                const QString n1 = m_Spreadsheet->string(row, NAME).simplified();
                const QString n2 = m_Spreadsheet->string(row, NAME+1).simplified();
                const QString cn = m_Spreadsheet->string(row, NAME+2).simplified();
                staff->FirstName.set(n1, staff->Nation.get());
                staff->SecondName.set(n2, staff->Nation.get());
                staff->CommonName.set(cn, staff->Nation.get());
            }

            /* First name in first column and second name in second column */
            else if(isNewStaff && m_ColumnAdjustment == 1)
            {
                const QString n1 = m_Spreadsheet->string(row, NAME).simplified();
                const QString n2 = m_Spreadsheet->string(row, NAME+1).simplified();

                // Common name
                if(n1.isEmpty() || n2.isEmpty())
                {
                    const QString cn = QString("%1%2").arg(n1).arg(n2);
                    staff->CommonName.set(cn, staff->Nation.get());
                }
                // Full name
                else
                {
                    staff->FirstName.set(n1, staff->Nation.get());
                    staff->SecondName.set(n2, staff->Nation.get());
                }
            }

            /* Full name in first column (i.e. m_ColumnAdjustment is zero) */
            else if(isNewStaff)
            {
                // Name buffer
                const QString n = m_Spreadsheet->string(row, NAME).simplified(); // Removes any whitespaces

                // Index of first space in text
                const int space = n.indexOf(" ", 0, Qt::CaseInsensitive);

                // Common name (no spaces in text)
                if(space < 0)
                {
                    staff->CommonName.set(n, staff->Nation.get());
                }
                // Full name
                else
                {
                    const QString n1 = n.left(space);
                    const QString n2 = n.right(n.size()-space-1);
                    staff->FirstName.set(n1, staff->Nation.get());
                    staff->SecondName.set(n2, staff->Nation.get());
                }
            }
        } else {
            // Columns positions
            const unsigned int posFurthestRightColumn = m_Spreadsheet->data()->at(row).size()-1;  // Furthest right column
            const unsigned int posOutOfRange = posFurthestRightColumn;
            const unsigned int posReserveDivisionId = posFurthestRightColumn - 1;
            const unsigned int posDivisionId = posFurthestRightColumn - 2;
            const unsigned int posLastDivisionId = posFurthestRightColumn - 3;
            const unsigned int posClubId = posFurthestRightColumn - 4;

            // Determine relevant club ID to use
            int clubId = m_Spreadsheet->number(row, posClubId);

            // Skip any invalid club Ids
            if(clubId == Spreadsheet::NOT_A_NUMBER || !(clubId >= VALID)) {
                continue;
            }

            // Skip any rows with out of range value
            if(!m_Spreadsheet->string(row, posOutOfRange).isEmpty()){
                continue;
            }

            // Skip any rows with bad division ids
            int lastDivisionId = m_Spreadsheet->number(row, posLastDivisionId);
            if(lastDivisionId == NO_MATCH || lastDivisionId == Spreadsheet::NOT_A_NUMBER) {
                continue;
            }
            int divisionId = m_Spreadsheet->number(row, posDivisionId);
            if(divisionId == NO_MATCH || divisionId == Spreadsheet::NOT_A_NUMBER) {
                continue;
            }
            int reserveDivisionId = m_Spreadsheet->number(row, posReserveDivisionId);
            if(reserveDivisionId == NO_MATCH || reserveDivisionId == Spreadsheet::NOT_A_NUMBER) {
                continue;
            }

            /* Edit existing club */
            // Club pointer
            Club *club = &(Club::dbDom[clubId]);

            // Club divisions
            if(lastDivisionId >= VALID) {
                club->LastDivision.set(lastDivisionId);
            }
            if(divisionId >= VALID) {
                club->Division.set(divisionId);
            }
            if(reserveDivisionId >= VALID) {
                club->ReserveDivision.set(reserveDivisionId);
            }

            // Reputation
            QString newRep = m_Spreadsheet->string(row, CLUB_REPUTATION);
            if(!newRep.isEmpty()){
                // multiply by 500 because the 1-20 values displayed in editor are actually 500 times lower than the actual values stored in the database
                club->Reputation = newRep.toInt()*500;
            }

            // Cash
            QString newCash = m_Spreadsheet->string(row, CLUB_CASH);
            if(!newCash.isEmpty()){
                club->Cash = newCash.toInt();
            }

            // Training
            QString newTraining = m_Spreadsheet->string(row, CLUB_TRAINING);
            if(!newTraining.isEmpty()){
                club->Training = newTraining.toInt();
            }

            // ProfessionalStatus
            QString newProfessionalStatus = m_Spreadsheet->string(row, CLUB_PROFESSIONAL_STATUS);
            if(!newProfessionalStatus.isEmpty()){
                club->ProfessionalStatus = newProfessionalStatus.toInt();
            }

            // PLC
            QString newPLC = m_Spreadsheet->string(row, CLUB_PLC);
            if(!newPLC.isEmpty()){
                club->PLC = newPLC.toInt();
            }

            // Attendance
            QString newAttendance = m_Spreadsheet->string(row, CLUB_ATTENDANCE);
            if(!newAttendance.isEmpty()){
                club->Attendance = newAttendance.toInt();
            }

            // MinAttendance
            QString newMinAttendance = m_Spreadsheet->string(row, CLUB_MIN_ATTENDANCE);
            if(!newMinAttendance.isEmpty()){
                club->MinAttendance = newMinAttendance.toInt();
            }

            // MaxAttendance
            QString newMaxAttendance = m_Spreadsheet->string(row, CLUB_MAX_ATTENDANCE);
            if(!newMaxAttendance.isEmpty()){
                club->MaxAttendance = newMaxAttendance.toInt();
            }

            // LastPosition
            QString newLastPosition = m_Spreadsheet->string(row, CLUB_LAST_POSITION);
            if(!newLastPosition.isEmpty()){
                club->LastPosition = newLastPosition.toInt();
            }
        }
    }

    // Stop timer
    this->setTimerLabels(timer.elapsed(), rowSize,
                         m_TimerItem[IMPORT_TOTAL],
                         m_TimerItem[IMPORT_PER_RECORD]);

    // Stop/complete the progress bar
    this->stopProgressBar();
    m_ProgressLabel->setText("Import Complete");

    // Enable and disable the relevant buttons
    m_CloseButton->setEnabled(true);
    m_ImportButton->setEnabled(false);
    m_SaveButton->setEnabled(true);
    m_SaveButton->setFocus();
}


// --- Process the data and identify the relevant ID numbers --- //
void TransferImporter::process(const QTime &timer)
{
    // Apply settings
    String::ignoreAccents(m_RadioButton[IGNORE_ACCENTS]->isChecked());

    // Counters
    int counterPlayerMatches = 0;
    int counterPlayerUnmatched = 0;
    int counterPlayerProtected = 0;
    int counterErrorRows = 0;
    int counterClubMatches = 0;
    int counterClubUnmatched = 0;

    // Progress bar
    if(m_IsPlayerSheet){
        m_ProgressBar->setRange(0, 4 + m_Spreadsheet->data()->size());
    } else {
        m_ProgressBar->setRange(0, 2 + m_Spreadsheet->data()->size());
    }

    // Hash tables
    QHash<QString, int> clubs;
    QHash<QString, int> nations;
    QHash<QString, int> staffMain;
    QHash<QString, int> staffFailSafe;    
    QHash<QString, int> divisions;    

    // Prefix/suffix table
    QVector<QString> clubPrefixSuffix;

    // VLookup
    VLookupHash vlookup(VLookupHash::CLUBS);

    // Club hash table
    this->incrementProgressBar("Creating club list");

    if(m_RadioButton[CLUB_NAMES_SHORT_AND_LONG]->isChecked() || !m_IsPlayerSheet)
        Club::createHash(clubs, true, !m_IsPlayerSheet);// Add DB long names (if the relevant RadioButton is checked or if its club sheet)

    if(m_IsPlayerSheet)
        Club::createHash(clubs, false);                 // Add DB short names only if its player sheet    
    if(m_RadioButton[CLUB_LOOKUP_ENABLED]->isChecked()){
        vlookup.load(clubs);                            // Add VLookup names
        vlookup.set(VLookupHash::CLUBS_PREFIX_SUFFIX);  // Club prefix/suffixes
        vlookup.load(clubPrefixSuffix);                 // Add prefix/suffixes
    }

    if(m_IsPlayerSheet) {
        // Add nation hash table only if its player sheet
        this->incrementProgressBar("Creating nation list");
        Nation::createHash(nations);    // Add DB names
        vlookup.set(VLookupHash::NATIONS);
        vlookup.load(nations);          // Add VLookup names
    } else {
        // Add comp names only if its club sheet
        this->incrementProgressBar("Creating club competition list");        
        ClubComp::createHash(divisions);        
    }

    if(m_IsPlayerSheet){
        // Staff hash tables
        this->incrementProgressBar("Creating staff list");
        Staff::createHash(staffMain, staffFailSafe, Staff::SCOPE_PLAYERS, m_YearAdjustment->value());

        // Load protected players list
        this->incrementProgressBar("Creating protected staff list");
        this->loadProtectedPlayersList(staffMain);

        // Add header columns
        m_Spreadsheet->addHeader("Staff ID", 0);
        m_Spreadsheet->addHeader("Nation1 ID", 0);
        m_Spreadsheet->addHeader("Nation2 ID", 0);
        m_Spreadsheet->addHeader("Club ID", 0);
        m_Spreadsheet->addHeader("Loan Club ID", 0);
        m_Spreadsheet->addHeader("Protected?", 0);        
        m_Spreadsheet->addHeader("Matched?", 0);
        m_Spreadsheet->addHeader("Errored?", 0);
    } else {
        // Add header columns
        m_Spreadsheet->addHeader("Club ID", 0);
        m_Spreadsheet->addHeader("Last Division ID", 0);
        m_Spreadsheet->addHeader("Division ID", 0);
        m_Spreadsheet->addHeader("Reserve Division ID", 0);
        m_Spreadsheet->addHeader("Out of Range?", 0);
    }

    // Prepare progress bar label
    m_ProgressLabel->setText("Processing spreadsheet");

    // Process the rows of data (loop by index for optimum speed)
    const unsigned int rowSize = static_cast<unsigned int>(m_Spreadsheet->data()->size());
    for(unsigned int row = 0; row < rowSize; ++row) {

        // Progress bar
        this->incrementProgressBar();

        if(m_IsPlayerSheet){
            // Staff
            int staffId = BLANK;
            QString staffText;

            // Set name (where the full name is in the first column)
            if(m_ColumnAdjustment == 0)
            {
                staffText = m_Spreadsheet->matchString(row, NAME);
            }

            // Set name where:
            // (1) the first name is in the first column;
            // (2) the second name is in the second column; and
            // (3) the common name is in the third column
            else
            {
                const QString firstname = m_Spreadsheet->matchString(row, NAME);
                const QString secondname = m_Spreadsheet->matchString(row, NAME+1);
                const QString commonname = m_Spreadsheet->matchString(row, NAME+2);

                // Use first name + surname if no common name is present
                if(commonname.isEmpty())
                {
                    staffText = QString("%1 %2")
                            .arg(firstname)
                            .arg(secondname);
                }
                // Use common name if one has been entered
                else
                {
                    staffText = commonname;
                }

            }

            // Match the staff against the database
            if(!staffText.isEmpty()) {  // Only proceed if the cell is not empty
                // Staff date of birth
                QString yob = m_Spreadsheet->string(row, DATE_OF_BIRTH).right(4);
                QString dob = m_Spreadsheet->matchDateString(row, DATE_OF_BIRTH);

                // Match the staff against the database
                staffId = staffMain.value(QString("%1 %2").arg(staffText).arg(dob), NO_MATCH);

                // Use failsafe hash (i.e. year of birth) if no match found using main hash (i.e. date of birth)
                // Note: removed as it was causing players to be considered equal if their names and YOB only were equal
                /*if(staffId == NO_MATCH)
                    staffId = staffFailSafe.value(QString("%1 %2").arg(staffText).arg(yob), NO_MATCH);*/
            }

            // Check for protected staff (vector will be empty if the setting is disabled)
            bool isProtected = (staffId >= VALID && m_ProtectedPlayers.contains(staffId));

            // Club
            int clubId = BLANK;
            QString clubText = m_Spreadsheet->matchString(row, CLUB);

            // Match the club against the database
            if(!clubText.isEmpty()) {   // Only proceed if the cell is not empty
                clubId = clubs.value(clubText, NO_MATCH);

                // Try prefix/suffixes if no exact match can be found
                if(clubId == NO_MATCH)
                    clubId = vlookup.lookupPrefixSuffix(clubText, clubPrefixSuffix, clubs, NO_MATCH);
            }

            // Loan Club
            int loanClubId = BLANK;
            QString loanClubText = m_Spreadsheet->matchString(row, ON_LOAN_FROM);

            // Match the club against the database
            if(!loanClubText.isEmpty()) {   // Only proceed if the cell is not empty
                loanClubId = clubs.value(loanClubText, NO_MATCH);

                // Try prefix/suffixes if no exact match can be found
                if(loanClubId == NO_MATCH)
                    loanClubId = vlookup.lookupPrefixSuffix(loanClubText, clubPrefixSuffix, clubs, NO_MATCH);
            }

            // Primary nation (blank means no nationality)
            QString primaryNationText = m_Spreadsheet->matchString(row, NATION_1);
            int primaryNationId = nations.value(primaryNationText, NO_MATCH);

            // Secondary nation (blank means no nationality)
            QString secondaryNationText = m_Spreadsheet->matchString(row, NATION_2);
            int secondaryNationId = nations.value(secondaryNationText, NO_MATCH);

            // Add data to spreadsheet
            m_Spreadsheet->add(row, staffId);
            m_Spreadsheet->add(row, primaryNationId);
            m_Spreadsheet->add(row, secondaryNationId);
            m_Spreadsheet->add(row, clubId);
            m_Spreadsheet->add(row, loanClubId);
            m_Spreadsheet->add(row, isProtected);

            // Increment counters
            if(staffId == NO_MATCH) {
                ++counterPlayerUnmatched;
                m_Spreadsheet->add(row, "Unmatched");
            }
            else {
                ++counterPlayerMatches;
                m_Spreadsheet->add(row, "Matched");
            }

            if(isProtected)
                ++counterPlayerProtected;

            if(clubId == NO_MATCH || loanClubId == NO_MATCH || primaryNationId == NO_MATCH || secondaryNationId == NO_MATCH) {
                ++counterErrorRows;
                m_Spreadsheet->add(row, "ERROR");
            } else {
                m_Spreadsheet->add(row, "");
            }
        } else {
            // Club
            int clubId = BLANK;
            QString clubText = m_Spreadsheet->matchString(row, CLUB_NAME);

            // Match the club against the database
            if(!clubText.isEmpty()) {   // Only proceed if the cell is not empty
                clubId = clubs.value(clubText, NO_MATCH);

                // Try prefix/suffixes if no exact match can be found
                if(clubId == NO_MATCH)
                    clubId = vlookup.lookupPrefixSuffix(clubText, clubPrefixSuffix, clubs, NO_MATCH);
            }

            // Division
            int lastDivisionId = BLANK;
            int divisionId = BLANK;
            int reserveDivisionId = BLANK;
            QString lastDivisionText = m_Spreadsheet->matchString(row, CLUB_LAST_DIVISION);
            QString divisionText = m_Spreadsheet->matchString(row, CLUB_DIVISION);
            QString reserveDivisionText = m_Spreadsheet->matchString(row, CLUB_RESERVE_DIVISION);

            // Match the last division against the database
            if(!lastDivisionText.isEmpty()) {   // Only proceed if the cell is not empty
                lastDivisionId = divisions.value(lastDivisionText, NO_MATCH);
            }

            // Match the division against the database
            if(!divisionText.isEmpty()) {   // Only proceed if the cell is not empty
                divisionId = divisions.value(divisionText, NO_MATCH);
            }

            // Match the reserve division against the database
            if(!reserveDivisionText.isEmpty()) {   // Only proceed if the cell is not empty
                reserveDivisionId = divisions.value(reserveDivisionText, NO_MATCH);
            }

            QString outOfRange = "";

            // Reputation
            if(!isWithinRange(m_Spreadsheet->string(row, CLUB_REPUTATION), 0, 20)){
                outOfRange.append("|BAD REPUTATION");
            }

            // Cash
            if(!isWithinRange(m_Spreadsheet->string(row, CLUB_CASH), -99999999, 149999999)){
                 outOfRange.append("|BAD CASH");
            }

            // Training
            if(!isWithinRange(m_Spreadsheet->string(row, CLUB_TRAINING), 0, 20)){
                 outOfRange.append("|BAD TRAINING");
            }

            // ProfessionalStatus
            if(!isWithinRange(m_Spreadsheet->string(row, CLUB_PROFESSIONAL_STATUS), 0, 3)){
                 outOfRange.append("|BAD PROFESSIONAL STATUS");
            }

            // PLC            
            if(!isWithinRange(m_Spreadsheet->string(row, CLUB_PLC), -1, 0)){
                 outOfRange.append("|BAD PLC");
            }

            // Attendance
            if(!isWithinRange(m_Spreadsheet->string(row, CLUB_ATTENDANCE), 0, 999999999)){
                 outOfRange.append("|BAD ATTENDANCE");
            }

            // MinAttendance
            if(!isWithinRange(m_Spreadsheet->string(row, CLUB_MIN_ATTENDANCE), 0, 999999999)){
                 outOfRange.append("|BAD MIN ATTENDANCE");
            }

            // MaxAttendance
            if(!isWithinRange(m_Spreadsheet->string(row, CLUB_MAX_ATTENDANCE), 0, 999999999)){
                 outOfRange.append("|BAD MAX ATTENDANCE");
            }

            // LastPosition
            if(!isWithinRange(m_Spreadsheet->string(row, CLUB_LAST_POSITION), 0, 50)){
                 outOfRange.append("|BAD LAST POSITION");
            }            

            // Add data to spreadsheet
            m_Spreadsheet->add(row, clubId);
            m_Spreadsheet->add(row, lastDivisionId);
            m_Spreadsheet->add(row, divisionId);
            m_Spreadsheet->add(row, reserveDivisionId);
            m_Spreadsheet->add(row, outOfRange);

            // Increment counters
            if(clubId < VALID)
                ++counterClubUnmatched;
            else
                ++counterClubMatches;

            if(clubId < VALID || lastDivisionId == NO_MATCH || divisionId == NO_MATCH || reserveDivisionId == NO_MATCH || !outOfRange.isEmpty())
                ++counterErrorRows;
        }
    }

    // Stop timer
    this->setTimerLabels(timer.elapsed(), m_Spreadsheet->data()->size(),
                         m_TimerItem[PROCESS_TOTAL],
                         m_TimerItem[PROCESS_PER_RECORD]);

    // Set summary items
    if(m_IsPlayerSheet){
        m_SummaryItem[SUMMARY_PLAYER_MATCHES]->setValue(counterPlayerMatches);
        m_SummaryItem[SUMMARY_PLAYER_MATCHES]->setText("Players matched");
    } else {
        m_SummaryItem[SUMMARY_PLAYER_MATCHES]->setValue(counterClubMatches);
        m_SummaryItem[SUMMARY_PLAYER_MATCHES]->setText("Clubs matched");
    }
    m_SummaryItem[SUMMARY_PLAYER_UNMATCHED]->setValue(counterPlayerUnmatched);
    m_SummaryItem[SUMMARY_PLAYER_PROTECTED]->setValue(counterPlayerProtected);
    m_SummaryItem[SUMMARY_ERRORS]->setValue(counterErrorRows);    

    // Stop/complete the progress bar
    this->stopProgressBar();
}


/* ================== */
/*      File I/O      */
/* ================== */

// --- Close CSV file --- //
void TransferImporter::onClose()
{
    // Reset summary items to zero
    for(SummaryItem *itr : m_SummaryItem)
        itr->setValue(0);

    // Reset the timer values
    this->resetTimerLabels();

    // Enable and disable the relevant buttons
    m_CloseButton->setEnabled(false);
    m_ImportButton->setEnabled(false);
    m_SaveButton->setEnabled(false);
    m_OpenButton->setEnabled(true);
    m_OpenButton->setFocus();
    this->resetProgressBar();

    // Enable the settings
    m_ExchangeRate->setEnabled(true);
    m_YearAdjustment->setEnabled(true);
    for(QRadioButton *itr : m_RadioButton)
        itr->setEnabled(true);

    // Delete the spreadsheet pointer
    delete m_Spreadsheet;
    m_Spreadsheet = nullptr; // Null pointer allows us to track whether the pointer is deleted
}

// --- Open CSV file --- //
void TransferImporter::onOpen()
{
    // Save the settings: Radio buttons
    for(unsigned int i = 0; i < RADIO_BUTTON_COUNT; ++i) {
        m_Settings->set(m_RadioButton[i]->isChecked(), m_SettingsPos + i);
    }

    // Save the settings: Exchange rate
    m_Settings->set(m_ExchangeRate->value(), m_SettingsPos + RADIO_BUTTON_COUNT);

    // Save the settings: Year adjustment
    m_Settings->set(m_YearAdjustment->value(), m_SettingsPos + RADIO_BUTTON_COUNT + 1);

    // Reset progress bar and disable settings
    this->resetProgressBar();
    m_ExchangeRate->setEnabled(false);
    m_YearAdjustment->setEnabled(false);
    for(QRadioButton *itr : m_RadioButton)
        itr->setEnabled(false);

    // Reset the timer values
    this->resetTimerLabels();

    // Spreadsheet
    m_Spreadsheet = new Spreadsheet(true);

    if(m_Spreadsheet->open(m_Settings->text(m_SettingImportPathPos), 1, true)) {

        // Update import folder path setting
        m_Settings->set(m_Spreadsheet->getFilePath(), m_SettingImportPathPos);

        // Temporarily disable the relevant buttons
        m_CloseButton->setEnabled(false);
        m_ImportButton->setEnabled(false);
        m_SaveButton->setEnabled(false);
        m_OpenButton->setEnabled(false);

        // Detect and set the column adjustment
        this->detectColumnAdjustment();

        // Process the data (and pass the spreadsheet's timer)
        this->process(m_Spreadsheet->time());
    }
    // Call onClose() if opening the spreadsheet was cancelled or failed
    else {
        this->onClose();
        return;
    }    

    // Enable and disable the relevant buttons
    m_CloseButton->setEnabled(true);
    m_ImportButton->setEnabled(true);
    m_SaveButton->setEnabled(true);
    m_SaveButton->setFocus();
}

// --- Save CSV file --- //
void TransferImporter::onSave()
{
    m_Spreadsheet->save();
}


/* ========================= */
/*      Progress Widget      */
/* ========================= */

// --- Create control widget --- //
void TransferImporter::createProgressWidget()
{
    // Widget & layout
    m_ProgressWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(m_ProgressWidget);

    // Progress label
    m_ProgressLabel = new QLabel(m_ProgressWidget);
    m_ProgressLabel->setObjectName("TextSummaryLarge");
    layout->addWidget(m_ProgressLabel, 0, Qt::AlignHCenter | Qt::AlignBottom);

    // Progress bar
    m_ProgressBar = new QProgressBar(m_ProgressWidget);
    //m_ProgressBar->setObjectName("TextSummaryLarge");
    layout->addWidget(m_ProgressBar);

    // Correct the label width so that it matches the bar width
    m_ProgressLabel->setMinimumWidth(m_ProgressBar->width());

    // Apply layout
    m_ProgressWidget->setLayout(layout);
}

// --- Increment the progress bar (for fast processing) --- //
void TransferImporter::incrementProgressBar()
{
    m_ProgressBar->setValue(m_ProgressBar->value()+1);
    QApplication::processEvents();
}

// --- Increment the progress bar (standard, with a label) --- //
void TransferImporter::incrementProgressBar(const QString &label)
{
    m_ProgressBar->setWindowModality(Qt::ApplicationModal);
    m_ProgressLabel->setText(label);
    m_ProgressBar->setValue(m_ProgressBar->value()+1);
    QApplication::processEvents();
}

// --- Reset the progress bar --- //
void TransferImporter::resetProgressBar()
{
    m_ProgressBar->setWindowModality(Qt::NonModal);
    m_ProgressLabel->clear();
    m_ProgressBar->setValue(0);
    m_ProgressBar->setEnabled(true);
}

// --- Stop the progress bar --- //
void TransferImporter::stopProgressBar()
{
    m_ProgressLabel->setText("Complete");
    m_ProgressBar->setValue(m_ProgressBar->maximum());
    m_ProgressBar->setEnabled(false);
}


/* ================================ */
/*      Protected Players List      */
/* ================================ */

// --- Create a blank protected players list --- //
void TransferImporter::createBlankProtectedPlayersList()
{
    QStringList header;
    header << QStringLiteral("First Name")
           << QStringLiteral("Second Name")
           << QStringLiteral("Date of Birth")
           << QStringLiteral("Staff ID");

    Spreadsheet s;
    s.addHeader(header);
    s.write(s_ProtectedPlayersFileName);
}

// --- Load protected players list --- //
void TransferImporter::loadProtectedPlayersList(QHash<QString, int> &staffList)
{
    // Clear the vector
    m_ProtectedPlayers.clear();

    // Abort if the setting is disabled
    if(m_RadioButton[PROTECTED_PLAYERS_DISABLED]->isChecked())
        return;

    // Load the spreadsheet (create a blank file if the file cannot be found)
    Spreadsheet s;
    s.setSilent(true);
    if(!s.read(s_ProtectedPlayersFileName)) {
        this->createBlankProtectedPlayersList();
        return;
    }

    enum ENUM_PROTECTED_COLUMN_POSITIONS {
        PR_FIRST_NAME = 0,
        PR_SECOND_NAME,
        PR_DATE_OF_BIRTH,
        PR_STAFF_ID
    };

    // Process the rows of data (loop by index for optimum speed)
    const unsigned int rowSize = static_cast<unsigned int>(s.data()->size());
    for(unsigned int row = 0; row < rowSize; ++row) {

        QString staff = QString("%1 %2 %3")
                .arg(s.matchString(row, PR_FIRST_NAME))
                .arg(s.matchString(row, PR_SECOND_NAME))
                .arg(s.matchDateString(row, PR_DATE_OF_BIRTH));

        // Match the staff against the database
        int id = staffList.value(staff, NO_MATCH);

        // Add id to spreadsheet
        s.set(row, PR_STAFF_ID, id);

        // Skip if no valid match found
        if(id < VALID)
            continue;
        else
            m_ProtectedPlayers.push_back(id);
    }

    // Save the modified spreadsheet
    s.write();

    // Sort the protected players list
    std::sort(m_ProtectedPlayers.begin(), m_ProtectedPlayers.end());
}


/* ========================= */
/*      Settings Widget      */
/* ========================= */

// --- Create settings widget --- //
void TransferImporter::createSettingsWidget()
{
    // Widget & layout
    m_SettingsWidget = new QWidget(this);
    m_SettingsWidget->setFixedWidth(250);
    QVBoxLayout *layout = new QVBoxLayout(m_SettingsWidget);
    layout->setContentsMargins(0,0,0,0);

    // Create radio buttons
    for(int i = 0; i < RADIO_BUTTON_COUNT; ++i)
        m_RadioButton.push_back(nullptr); // Ensure that all buttons are initialised otherwise use of "nullptr"
                                          // could cause a crash

    // Create heading and description labels
    for(int i = 0; i < SETTINGS_COUNT; ++i) {

        QGroupBox *group = new QGroupBox(m_SettingsWidget);
        group->setObjectName("TextSettingGroup");
        layout->addWidget(group);
        m_SettingsGroup.push_back(group);

        QLabel *desc = new QLabel(m_SettingsWidget);
        desc->setObjectName("TextSettingDescription");
        desc->setWordWrap(true);
        desc->setContentsMargins(0,0,0,0);
        m_SettingsDescription.push_back(desc);

        if(i < EXCHANGE_RATE) // Don't add a button group for Exchange Rate or Year Adjustment
            m_ButtonGroup.push_back(new QButtonGroup(m_SettingsWidget));
    }

    // Accent matching
    m_SettingsGroup[ACCENTS]->setTitle("Accent Matching");
    m_SettingsDescription[ACCENTS]->setText("Accented text matching sensitivity");
    m_RadioButton[IGNORE_ACCENTS] = new QRadioButton("Ignore accents");
    m_RadioButton[USE_ACCENTS] = new QRadioButton("Use accents");
    m_ButtonGroup[ACCENTS]->addButton(m_RadioButton[IGNORE_ACCENTS]);
    m_ButtonGroup[ACCENTS]->addButton(m_RadioButton[USE_ACCENTS]);

    QGridLayout *layoutAccents = new QGridLayout(m_SettingsGroup[ACCENTS]);
    layoutAccents->addWidget(m_SettingsDescription[ACCENTS], 0, 0, 1, 2);
    layoutAccents->addWidget(m_RadioButton[IGNORE_ACCENTS]);
    layoutAccents->addWidget(m_RadioButton[USE_ACCENTS]);
    m_SettingsGroup[ACCENTS]->setLayout(layoutAccents);

    // Club lookup
    m_SettingsGroup[CLUB_LOOKUP]->setTitle("Club Name Lookup");
    m_SettingsDescription[CLUB_LOOKUP]->setText("Use lookup data from vlookup_clubs.csv");
    m_RadioButton[CLUB_LOOKUP_DISABLED] = new QRadioButton("Disabled");
    m_RadioButton[CLUB_LOOKUP_ENABLED] = new QRadioButton("Enabled");
    m_ButtonGroup[CLUB_LOOKUP]->addButton(m_RadioButton[CLUB_LOOKUP_DISABLED]);
    m_ButtonGroup[CLUB_LOOKUP]->addButton(m_RadioButton[CLUB_LOOKUP_ENABLED]);

    QGridLayout *layoutClubLookup = new QGridLayout(m_SettingsGroup[CLUB_LOOKUP]);
    layoutClubLookup->addWidget(m_SettingsDescription[CLUB_LOOKUP], 0, 0, 1, 2);
    layoutClubLookup->addWidget(m_RadioButton[CLUB_LOOKUP_ENABLED]);
    layoutClubLookup->addWidget(m_RadioButton[CLUB_LOOKUP_DISABLED]);
    m_SettingsGroup[CLUB_LOOKUP]->setLayout(layoutClubLookup);

    // Club names
    m_SettingsGroup[CLUB_NAMES]->setTitle("Club Name Source");
    m_SettingsDescription[CLUB_NAMES]->setText("Match against long and/or short club names");
    m_RadioButton[CLUB_NAMES_SHORT_ONLY] = new QRadioButton("Short Only");
    m_RadioButton[CLUB_NAMES_SHORT_AND_LONG] = new QRadioButton("Short and Long");
    m_ButtonGroup[CLUB_NAMES]->addButton(m_RadioButton[CLUB_NAMES_SHORT_ONLY]);
    m_ButtonGroup[CLUB_NAMES]->addButton(m_RadioButton[CLUB_NAMES_SHORT_AND_LONG]);

    QGridLayout *layoutClubNames = new QGridLayout(m_SettingsGroup[CLUB_NAMES]);
    layoutClubNames->addWidget(m_SettingsDescription[CLUB_NAMES], 0, 0, 1, 2);
    layoutClubNames->addWidget(m_RadioButton[CLUB_NAMES_SHORT_ONLY]);
    layoutClubNames->addWidget(m_RadioButton[CLUB_NAMES_SHORT_AND_LONG]);
    m_SettingsGroup[CLUB_NAMES]->setLayout(layoutClubNames);

    // First name lookup
    m_SettingsGroup[FIRST_NAME_LOOKUP]->setTitle("First Name Lookup");
    m_SettingsDescription[FIRST_NAME_LOOKUP]->setText("Attempt to match using similar first names");
    m_RadioButton[FIRST_NAME_LOOKUP_DISABLED] = new QRadioButton("Disabled");
    m_RadioButton[FIRST_NAME_LOOKUP_ENABLED] = new QRadioButton("Enabled");
    m_ButtonGroup[FIRST_NAME_LOOKUP]->addButton(m_RadioButton[FIRST_NAME_LOOKUP_ENABLED]);
    m_ButtonGroup[FIRST_NAME_LOOKUP]->addButton(m_RadioButton[FIRST_NAME_LOOKUP_DISABLED]);

    QGridLayout *layoutFirstNameLookup = new QGridLayout(m_SettingsGroup[FIRST_NAME_LOOKUP]);
    layoutFirstNameLookup->addWidget(m_SettingsDescription[FIRST_NAME_LOOKUP], 0, 0, 1, 2);
    layoutFirstNameLookup->addWidget(m_RadioButton[FIRST_NAME_LOOKUP_ENABLED]);
    layoutFirstNameLookup->addWidget(m_RadioButton[FIRST_NAME_LOOKUP_DISABLED]);
    m_SettingsGroup[FIRST_NAME_LOOKUP]->setLayout(layoutFirstNameLookup);

    // Protected players
    m_SettingsGroup[PROTECTED_PLAYERS]->setTitle("Protected Players");
    m_SettingsDescription[PROTECTED_PLAYERS]->setText("Use lookup data from vlookup_protected_players.csv");
    m_RadioButton[PROTECTED_PLAYERS_DISABLED] = new QRadioButton("Disabled");
    m_RadioButton[PROTECTED_PLAYERS_ENABLED] = new QRadioButton("Enabled");
    m_ButtonGroup[PROTECTED_PLAYERS]->addButton(m_RadioButton[PROTECTED_PLAYERS_ENABLED]);
    m_ButtonGroup[PROTECTED_PLAYERS]->addButton(m_RadioButton[PROTECTED_PLAYERS_DISABLED]);

    QGridLayout *layoutProtectedPlayers = new QGridLayout(m_SettingsGroup[PROTECTED_PLAYERS]);
    layoutProtectedPlayers->addWidget(m_SettingsDescription[PROTECTED_PLAYERS], 0, 0, 1, 2);
    layoutProtectedPlayers->addWidget(m_RadioButton[PROTECTED_PLAYERS_ENABLED]);
    layoutProtectedPlayers->addWidget(m_RadioButton[PROTECTED_PLAYERS_DISABLED]);
    m_SettingsGroup[PROTECTED_PLAYERS]->setLayout(layoutProtectedPlayers);

    // Exchange rate
    m_SettingsGroup[EXCHANGE_RATE]->setTitle("Exchange Rate");
    m_SettingsDescription[EXCHANGE_RATE]->setText("The number of £££ per 1 unit of currency");
    m_ExchangeRate = new QDoubleSpinBox(m_SettingsGroup[EXCHANGE_RATE]);
    m_ExchangeRate->setDecimals(5);
    m_ExchangeRate->setSingleStep(0.00001);
    m_ExchangeRate->setPrefix("£ ");

    QVBoxLayout *layoutExchangeRate = new QVBoxLayout(m_SettingsGroup[EXCHANGE_RATE]);
    layoutExchangeRate->addWidget(m_SettingsDescription[EXCHANGE_RATE]);
    layoutExchangeRate->addWidget(m_ExchangeRate);
    m_SettingsGroup[EXCHANGE_RATE]->setLayout(layoutExchangeRate);

    // Year adjustment
    m_SettingsGroup[YEAR_ADJUSTMENT]->setTitle("Year Adjustment");
    m_SettingsDescription[YEAR_ADJUSTMENT]->setText("Years in file minus years in DB = value below");
    m_YearAdjustment = new QSpinBox(m_SettingsGroup[YEAR_ADJUSTMENT]);

    QVBoxLayout *layoutYearAdjustment = new QVBoxLayout(m_SettingsGroup[YEAR_ADJUSTMENT]);
    layoutYearAdjustment->addWidget(m_SettingsDescription[YEAR_ADJUSTMENT]);
    layoutYearAdjustment->addWidget(m_YearAdjustment);
    m_SettingsGroup[YEAR_ADJUSTMENT]->setLayout(layoutYearAdjustment);

    // Apply layout
    m_SettingsWidget->setLayout(layout);

    // Initialise the default settings
    this->applyDefaultSettings();

    // Apply any saved settings
    if(m_Settings->count() > m_SettingsPos) {
        // Radio buttons
        for(unsigned int i = 0; i < RADIO_BUTTON_COUNT; ++i)
            m_RadioButton[i]->setChecked(m_Settings->get(m_SettingsPos + i).toBool());

        // Exchange rate
        m_ExchangeRate->setValue(m_Settings->get(m_SettingsPos + RADIO_BUTTON_COUNT).toDouble());

        // Year adjustment
        m_YearAdjustment->setValue(m_Settings->get(m_SettingsPos + RADIO_BUTTON_COUNT + 1).toInt());
    }
}

// --- Apply/initialise the defaul settings --- //
void TransferImporter::applyDefaultSettings()
{
    m_ExchangeRate->setValue(1);
    m_YearAdjustment->setValue(0);
    m_RadioButton[IGNORE_ACCENTS]->setChecked(true);
    m_RadioButton[CLUB_LOOKUP_ENABLED]->setChecked(true);
    m_RadioButton[CLUB_NAMES_SHORT_AND_LONG]->setChecked(true);
    m_RadioButton[FIRST_NAME_LOOKUP_ENABLED]->setChecked(true);
    m_RadioButton[PROTECTED_PLAYERS_ENABLED]->setChecked(true);
}


/* ===================== */
/*      Spreadsheet      */
/* ===================== */

// --- Detect column adjustment (i.e. whether one or three name columns) --- //
void TransferImporter::detectColumnAdjustment()
{
    // Check if we are parsing club sheet or player sheet by checking the first column
    const QString sampleColA = m_Spreadsheet->headerString(0,0).toLower();

    // This means we are parsing club sheet
    if(sampleColA.contains("club")) {
        m_IsPlayerSheet = false;
        m_ColumnAdjustment = 0;

        // Set the adjusted column positions
        CLUB_NAME = COL_CLUB_NAME + m_ColumnAdjustment;
        CLUB_REPUTATION = COL_CLUB_REPUTATION + m_ColumnAdjustment;
        CLUB_CASH = COL_CLUB_CASH + m_ColumnAdjustment;
        CLUB_TRAINING = COL_CLUB_TRAINING + m_ColumnAdjustment;
        CLUB_PROFESSIONAL_STATUS = COL_CLUB_PROFESSIONAL_STATUS + m_ColumnAdjustment;
        CLUB_PLC = COL_CLUB_PLC + m_ColumnAdjustment;
        CLUB_ATTENDANCE = COL_CLUB_ATTENDANCE + m_ColumnAdjustment;
        CLUB_MIN_ATTENDANCE = COL_CLUB_MIN_ATTENDANCE + m_ColumnAdjustment;
        CLUB_MAX_ATTENDANCE = COL_CLUB_MAX_ATTENDANCE + m_ColumnAdjustment;
        CLUB_LAST_POSITION = COL_CLUB_LAST_POSITION + m_ColumnAdjustment;
        CLUB_LAST_DIVISION = COL_CLUB_LAST_DIVISION + m_ColumnAdjustment;
        CLUB_DIVISION = COL_CLUB_DIVISION + m_ColumnAdjustment;
        CLUB_RESERVE_DIVISION = COL_CLUB_RESERVE_DIVISION + m_ColumnAdjustment;


        return;
    }

    //Otherwise, we are parsing player sheet
    m_IsPlayerSheet = true;

    // Use the second column heading as the sample
    const QString sampleColB = m_Spreadsheet->headerString(0,1).toLower();
    const QString sampleColC = m_Spreadsheet->headerString(0,2).toLower();

    // If the third column heading contains a "name" type heading then the first two columns contain forename and surname.
    // This means a column adjustment of two because the date of birth is in the fourth column, etc.
    // | Col 0     | Col 1    | Col 2     | Col 1 + 2 = 3 | Col 2 + 2 = 4       |
    // | Forename  | Surname  | Commonname| Date of Birth | Primary Nationality |
    if(sampleColC.contains("name")) {
        m_ColumnAdjustment = 2;
    }

    // If the second column heading contains a "surname" type heading then the first column contains the forename only.
    // This means a column adjustment of two because the date of birth is in the fourth column, etc.
    // | Col 0     | Col 1    | Col 1 + 1 = 2 | Col 2 + 1 = 3       |
    // | Forename  | Surname  | Date of Birth | Primary Nationality |
    else if(sampleColB.contains("name")) {
        m_ColumnAdjustment = 1;
    }

    // Default adjustment of zero (i.e. assumption that the second column contains the date of birth)
    else {
        m_ColumnAdjustment = 0;
    }

    // Set the adjusted column positions
    NAME = COL_NAME;    // This column is never adjusted as it is always first
    DATE_OF_BIRTH = COL_DATE_OF_BIRTH + m_ColumnAdjustment;
    NATION_1 = COL_NATION_1 + m_ColumnAdjustment;
    NATION_2 = COL_NATION_2 + m_ColumnAdjustment;
    CONTRACT_START = COL_CONTRACT_START + m_ColumnAdjustment;
    CONTRACT_END = COL_CONTRACT_END + m_ColumnAdjustment;
    VALUE = COL_VALUE + m_ColumnAdjustment;
    CLUB = COL_CLUB + m_ColumnAdjustment;
    ON_LOAN_FROM = COL_ON_LOAN_FROM + m_ColumnAdjustment;
    GK = COL_GK + m_ColumnAdjustment;
    SW = COL_SW + m_ColumnAdjustment;
    D = COL_D + m_ColumnAdjustment;
    DM = COL_DM + m_ColumnAdjustment;
    M = COL_M + m_ColumnAdjustment;
    AM = COL_AM + m_ColumnAdjustment;
    ST = COL_ST + m_ColumnAdjustment;
    WB = COL_WB + m_ColumnAdjustment;
    FR = COL_FR + m_ColumnAdjustment;
    SIDE_RIGHT = COL_SIDE_RIGHT + m_ColumnAdjustment;
    SIDE_LEFT = COL_SIDE_LEFT + m_ColumnAdjustment;
    SIDE_CENTRE = COL_SIDE_CENTRE + m_ColumnAdjustment;
    FOOT_RIGHT = COL_FOOT_RIGHT + m_ColumnAdjustment;
    FOOT_LEFT = COL_FOOT_LEFT + m_ColumnAdjustment;
    INT_CAPS = COL_INT_CAPS + m_ColumnAdjustment;
    INT_GOALS = COL_INT_GOALS + m_ColumnAdjustment;
    ABILITY_CURRENT = COL_ABILITY_CURRENT + m_ColumnAdjustment;
    ABILITY_POTENTIAL = COL_ABILITY_POTENTIAL + m_ColumnAdjustment;
    REPUTATION_HOME = COL_REPUTATION_HOME + m_ColumnAdjustment;
    REPUTATION_CURRENT = COL_REPUTATION_CURRENT + m_ColumnAdjustment;
    SQUAD_NUMBER = COL_SQUAD_NUMBER + m_ColumnAdjustment;
}

// --- returns true if the stringValue is numerical and within range or if it is empty, false otherwise --- //
bool TransferImporter::isWithinRange(QString stringValue, int minRange, int maxRange){
    if(stringValue.isEmpty()){
        return true;
    }

    bool ok;
    int intValue = stringValue.toInt(&ok);
    if(!ok){
        return false;
    }

    return (intValue >= minRange && intValue <= maxRange);
}


/* ======================= */
/*      Status Widget      */
/* ======================= */

// --- Create status widget --- //
void TransferImporter::createStatusWidget()
{
    // Widget & layout
    m_StatusWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(m_StatusWidget);

    // Add summary items
    for(int i = 0; i < SUMMARY_COUNT; ++i) {
        SummaryItem *tmp = new SummaryItem(m_StatusWidget);
        layout->addWidget(tmp, 0, i);
        m_SummaryItem.push_back(tmp);
    }

    // Add timer items
    for(int i = 0; i < TIMER_COUNT; ++i) {
        SummaryItem *tmp = new SummaryItem(this);
        layout->addWidget(tmp, 1, i);
        m_TimerItem.push_back(tmp);
    }

    m_StatusWidget->setLayout(layout);

    // Player matches
    m_SummaryItem[SUMMARY_PLAYER_MATCHES]->setType(SummaryItem::SUCCESS);
    m_SummaryItem[SUMMARY_PLAYER_MATCHES]->setText("Players (or Clubs) matched");

    // Player matches
    m_SummaryItem[SUMMARY_PLAYER_PROTECTED]->setType(SummaryItem::INFO);
    m_SummaryItem[SUMMARY_PLAYER_PROTECTED]->setText("Protected players");

    // Players not matched
    m_SummaryItem[SUMMARY_PLAYER_UNMATCHED]->setType(SummaryItem::INFO);
    m_SummaryItem[SUMMARY_PLAYER_UNMATCHED]->setText("Players to be created");

    // Club/nation errors
    m_SummaryItem[SUMMARY_ERRORS]->setType(SummaryItem::ERROR);
    m_SummaryItem[SUMMARY_ERRORS]->setText("Rows with errors");    

    // Import time per record
    m_TimerItem[IMPORT_PER_RECORD]->setType(SummaryItem::SUBTLE);
    m_TimerItem[IMPORT_PER_RECORD]->setText("Import\n(Per Record)");

    // Import time total
    m_TimerItem[IMPORT_TOTAL]->setType(SummaryItem::SUBTLE);
    m_TimerItem[IMPORT_TOTAL]->setText("Import\n(Total)");

    // Process time per record
    m_TimerItem[PROCESS_PER_RECORD]->setType(SummaryItem::SUBTLE);
    m_TimerItem[PROCESS_PER_RECORD]->setText("Loading\n(Per Record)");

    // Process time total
    m_TimerItem[PROCESS_TOTAL]->setType(SummaryItem::SUBTLE);
    m_TimerItem[PROCESS_TOTAL]->setText("Loading\n(Total)");
}


/* =============== */
/*      Timer      */
/* =============== */

// --- Reset timer labels --- //
void TransferImporter::resetTimerLabels()
{
    m_TimerItem[IMPORT_PER_RECORD]->setValue("--:--.---");
    m_TimerItem[IMPORT_TOTAL]->setValue("--:--.---");
    m_TimerItem[PROCESS_PER_RECORD]->setValue("--:--.---");
    m_TimerItem[PROCESS_TOTAL]->setValue("--:--.---");
}

// --- Calculate time and set timer labels --- //
void TransferImporter::setTimerLabels(const int &msecs, const int &recordCount, SummaryItem *total, SummaryItem *perRecord)
{
    QString tmp;
    // Total time
    QTime time(0,0,0);
    time = time.addMSecs(msecs);
    tmp = QString("%1:%2.%3")
            .arg(time.minute(), 2, 10, QLatin1Char('0'))
            .arg(time.second(), 2, 10, QLatin1Char('0'))
            .arg(time.msec(), 3, 10, QLatin1Char('0'));
    total->setValue(tmp);

    // Time per record
    time.setHMS(0,0,0,0);
    time = time.addMSecs(msecs / recordCount);
    tmp = QString("%1:%2.%3")
            .arg(time.minute(), 2, 10, QLatin1Char('0'))
            .arg(time.second(), 2, 10, QLatin1Char('0'))
            .arg(time.msec(), 3, 10, QLatin1Char('0'));

    // If the average time per record is less than 1msec then indicate this with the "~" (approximate) symbol
    if((msecs / recordCount) < 1)
        tmp.prepend("~");

    perRecord->setValue(tmp);
}
