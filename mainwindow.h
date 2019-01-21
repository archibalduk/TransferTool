#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QAbstractTableModel>
#include <QMainWindow>
#include <QTextEdit>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableView>
#include <QToolBar>
#include <QToolButton>

// Importer headers
#include "importer/transfer_importer.h"

// Database headers
#include "database/database.h"

// Other custom headers
#include "exporter/exporter.h"
#include "settings.h"

namespace Ui {
class MainWindow;
}

// --- Main window --- //
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // File I/O
    void onOpen();
    void onSave();
    void onSaveAs();

    // Spreadsheet
    void onReadBufferChange(const int &i);

private:
    // Database
    Database m_Database;

    // Settings
    Settings m_Settings;

    enum ENUM_SETTINGS {
        DATABASE_PATH,
        EXPORT_PATH,
        IMPORT_PATH,
        TRANSFER_IMPORTER_SETTINGS_START_POS,
        TRANSFER_IMPORTER_SETTINGS_END_POS = (TransferImporter::SETTINGS_COUNT - 1)
    };

    // Spreadsheet
    QSpinBox *m_SpinBoxRowReadBufferSize;

    // Tables
    QTableView *m_IndexTable;

    // Tool buttons
    QToolButton *m_ButtonFileOpen;
    QToolButton *m_ButtonFileSave;
    QToolButton *m_ButtonFileSaveAs;

    // User interfece (functions)
    void createExporter(Exporter *e);
    void createStatusBar();
    void createToolBarButtons();
    void createWindow();
    void showTabs();

    // User interface (members)
    QTextEdit *m_IntroLabel;
    QTabWidget *m_TabWidget;
    QToolBar *m_ToolBar;
    Ui::MainWindow *ui;
};

#endif // MAIN_WINDOW_H
