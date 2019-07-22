#ifndef TRANSFER_IMPORTER_H
#define TRANSFER_IMPORTER_H

// Qt headers
#include <QButtonGroup>
#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTime>
#include <QVector>
#include <QWidget>

// Custom headers
#include "settings.h"
#include "spreadsheet/spreadsheet.h"
#include "ui/summary_item.h"

// --- Transfer importer and player creator --- //
class TransferImporter : public QWidget
{
    Q_OBJECT

public:
    // Constructor
    TransferImporter(Settings &settings,
                     int posSettings,
                     int posImportPath,
                     QWidget *parent = 0);

    // Settings
    enum ENUM_SETTINGS {
        ACCENTS,
        CLUB_LOOKUP,
        CLUB_NAMES,
        FIRST_NAME_LOOKUP,
        PROTECTED_PLAYERS,
        EXCHANGE_RATE,
        YEAR_ADJUSTMENT,
        SETTINGS_COUNT
    };

    enum ENUM_RADIO_BUTTONS {
        IGNORE_ACCENTS,
        USE_ACCENTS,
        CLUB_LOOKUP_DISABLED,
        CLUB_LOOKUP_ENABLED,
        CLUB_NAMES_SHORT_ONLY,
        CLUB_NAMES_SHORT_AND_LONG,
        FIRST_NAME_LOOKUP_DISABLED,
        FIRST_NAME_LOOKUP_ENABLED,
        PROTECTED_PLAYERS_DISABLED,
        PROTECTED_PLAYERS_ENABLED,
        RADIO_BUTTON_COUNT
    };

private slots:
    // Data functions
    void onImport();

    // File I/O
    void onClose();
    void onOpen();
    void onSave();

private:
    // Data functions
    void process(const QTime &timer);

    // Left-hand pane: Control widget
    QPushButton *m_CloseButton;
    QPushButton *m_ImportButton;
    QPushButton *m_OpenButton;
    QPushButton *m_SaveButton;
    QWidget *m_ControlWidget;
    void createControlWidget();

    // Left-hand pane: Progress widget
    QProgressBar *m_ProgressBar;
    QLabel *m_ProgressLabel;
    QWidget *m_ProgressWidget;
    void createProgressWidget();
    void incrementProgressBar();
    void incrementProgressBar(const QString &label);
    void resetProgressBar();
    void stopProgressBar();

    // Left-hand pane: Status widget
    QVector<SummaryItem*> m_SummaryItem;
    QWidget *m_StatusWidget;
    void createStatusWidget();

    enum ENUM_SUMMARY_ITEMS {
        SUMMARY_PLAYER_MATCHES,
        SUMMARY_PLAYER_UNMATCHED,
        SUMMARY_PLAYER_PROTECTED,
        SUMMARY_ERRORS,
        SUMMARY_COUNT
    };

    // Protected players
    QVector<int> m_ProtectedPlayers;
    static QString s_ProtectedPlayersFileName;
    void createBlankProtectedPlayersList();
    void loadProtectedPlayersList(QHash<QString, int> &staffList);

    // Right-hand pane: Settings widget
    QWidget *m_SettingsWidget;
    void applyDefaultSettings();
    void createSettingsWidget();

    // Settings
    Settings *m_Settings;
    const int m_SettingsPos;
    const int m_SettingImportPathPos;
    QVector<QButtonGroup*> m_ButtonGroup;
    QVector<QRadioButton*> m_RadioButton;
    QVector<QLabel*> m_SettingsDescription;
    QVector<QGroupBox*> m_SettingsGroup;
    QDoubleSpinBox *m_ExchangeRate;
    QSpinBox *m_YearAdjustment;

    // Timer
    QVector<SummaryItem*> m_TimerItem;
    void resetTimerLabels();
    void setTimerLabels(const int &msecs, const int &recordCount, SummaryItem *total, SummaryItem *perRecord);

    enum ENUM_TIMER_LABELS {
        PROCESS_PER_RECORD,
        PROCESS_TOTAL,
        IMPORT_PER_RECORD,
        IMPORT_TOTAL,
        TIMER_COUNT
    };

    // Spreadsheet
    Spreadsheet *m_Spreadsheet;
    void detectColumnAdjustment();
    bool m_IsPlayerSheet;

    // Columns
    unsigned int m_ColumnAdjustment;

    // Unadjusted column positions for player sheet
    enum ENUM_COLUMN_POSITIONS {
        COL_NAME,
        COL_DATE_OF_BIRTH,
        COL_NATION_1,
        COL_NATION_2,
        COL_CONTRACT_START,
        COL_CONTRACT_END,
        COL_VALUE,
        COL_CLUB,
        COL_ON_LOAN_FROM,
        COL_GK,
        COL_SW,
        COL_D,
        COL_DM,
        COL_M,
        COL_AM,
        COL_ST,
        COL_WB,
        COL_FR,
        COL_SIDE_RIGHT,
        COL_SIDE_LEFT,
        COL_SIDE_CENTRE,
        COL_FOOT_RIGHT,
        COL_FOOT_LEFT,
        COL_INT_CAPS,
        COL_INT_GOALS,
        COL_ABILITY_CURRENT,
        COL_ABILITY_POTENTIAL,
        COL_REPUTATION_HOME,
        COL_REPUTATION_CURRENT,
        COL_SQUAD_NUMBER
    };

    // Unadjusted column positions for club sheet
    enum ENUM_COLUMN_POSITIONS_CLUB {
        COL_CLUB_NAME,
        COL_CLUB_REPUTATION
    };

    // Adjusted column positions for player sheet
    unsigned short NAME;
    unsigned short DATE_OF_BIRTH;
    unsigned short NATION_1;
    unsigned short NATION_2;
    unsigned short CONTRACT_START;
    unsigned short CONTRACT_END;
    unsigned short VALUE;
    unsigned short CLUB;
    unsigned short ON_LOAN_FROM;
    unsigned short GK;
    unsigned short SW;
    unsigned short D;
    unsigned short DM;
    unsigned short M;
    unsigned short AM;
    unsigned short ST;
    unsigned short WB;
    unsigned short FR;
    unsigned short SIDE_RIGHT;
    unsigned short SIDE_LEFT;
    unsigned short SIDE_CENTRE;
    unsigned short FOOT_RIGHT;
    unsigned short FOOT_LEFT;
    unsigned short INT_CAPS;
    unsigned short INT_GOALS;
    unsigned short ABILITY_CURRENT;
    unsigned short ABILITY_POTENTIAL;
    unsigned short REPUTATION_HOME;
    unsigned short REPUTATION_CURRENT;
    unsigned short SQUAD_NUMBER;

    // adjusted column positions for club sheet
    unsigned short CLUB_NAME;
    unsigned short CLUB_REPUTATION;
};

#endif // TRANSFER_IMPORTER_H
