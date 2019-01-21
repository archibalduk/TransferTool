#include "mainwindow.h"
#include "ui_mainwindow.h"

// Qt headers
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>
#include <QIcon>
#include <QTextStream>

// Application headers
#include "spreadsheet/spreadsheet.h"

// Database headers
#include "database/city.h"
#include "database/club.h"
#include "database/index.h"
#include "database/staff.h"

// Model headers
#include "models/index_model.h"


/* ===================== */
/*      Main Window      */
/* ===================== */

// --- Default constructor --- //
// --- THIS IS THE ESSENTIALLY THE ENTRY POINT OF THE PROGRAM --- //
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    // Setup the .ui
    ui->setupUi(this);

    // Application details
    this->setWindowTitle(QString("%1 v%2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
    qApp->setWindowIcon(QIcon("cmxf.ico"));

    // Initialise the window
    this->createWindow();

    // Add the toolbar buttons
    this->createToolBarButtons();

    // Initialise the status bar
    this->createStatusBar();
}

// --- Default destructor --- //
MainWindow::~MainWindow()
{
    delete ui;
}


/* ================== */
/*      File I/O      */
/* ================== */

// --- Slot: Open --- //
void MainWindow::onOpen()
{
    QString f = QFileDialog::getOpenFileName(this, tr("Open Database"),
                                             m_Settings.text(DATABASE_PATH),
                                             "CM 01/02 Database (index.dat)");

    // Abort if Open dialog cancelled
    if(f.isEmpty())
        return;
    else
        m_Settings.set(f, DATABASE_PATH);

    // Load the database
    if(m_Database.load(f)) {
        this->showTabs();
    }
}

// --- Slot: Save --- //
void MainWindow::onSave()
{
    const bool result = m_Database.save(m_Settings.text(DATABASE_PATH));

    if(result)
        QMessageBox::information(this,
                                 QStringLiteral("Database Saved"),
                                 QString("The database was successfully saved to %1").arg(m_Settings.text(DATABASE_PATH)));
    else
        QMessageBox::critical(this,
                                 QStringLiteral("Error"),
                                 QString("An error was encountered when attempting to save to %1")
                                    .arg(m_Settings.text(DATABASE_PATH)));
}

// --- Slot: Save As --- //
void MainWindow::onSaveAs()
{
    QString f = QFileDialog::getSaveFileName(this, tr("Save Database"),
                                             m_Settings.text(DATABASE_PATH),
                                             "CM 01/02 Database (index.dat)");

    // Abort if Save dialog cancelled
    if(f.isEmpty())
        return;
    else {
        m_Settings.set(f, DATABASE_PATH);
        this->onSave();
    }
}


/* ===================== */
/*      Spreadsheet      */
/* ===================== */

// --- Slot: Read buffer change --- //
void MainWindow::onReadBufferChange(const int &i)
{
    Spreadsheet::setReadBufferSize(i);
}


/* ======================== */
/*      User Interface      */
/* ======================== */

// --- Create the exporter user interface --- //
void MainWindow::createExporter(Exporter *e)
{    
    e->add(&City::exportData,
           "Cities",
           "List of cities.");

    e->add(&Club::exportData,
           "Clubs",
           "List of clubs.");

    e->add(&Club::exportRosters,
           "Club Rosters",
           "Club roster list in the form of Staff Id numbers. This is primarily for testing purposes.");

    e->add(&Staff::exportExtendedList,
           "Staff List (Extended)",
           "Staff list with extended data.");

    e->add(&Staff::exportTransferList,
           "Staff List (Transfer Importer Format)",
           "Staff list formatted as a Transfer Importer spreadsheet.");

    e->add(&Staff::exportTransferList_NoAccents,
           "Staff List (Transfer Importer Format) (NO ACCENTS)",
           "Staff list formatted as a Transfer Importer spreadsheet (no accents in staff and club names).");
}

// --- Create the status bar --- //
void MainWindow::createStatusBar()
{

}

// --- Create the toolbar buttons --- //
void MainWindow::createToolBarButtons()
{
    // Create the toolbar
    m_ToolBar = new QToolBar(this);
    this->addToolBar(m_ToolBar);
    m_ToolBar->setFloatable(false);
    m_ToolBar->setMovable(true);

    // Open
    m_ButtonFileOpen = new QToolButton(m_ToolBar);
    m_ButtonFileOpen->setIcon(QIcon(":/buttons/document-open.png"));
    m_ButtonFileOpen->setText("Open");
    m_ButtonFileOpen->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_ToolBar->addWidget(m_ButtonFileOpen);
    QObject::connect(m_ButtonFileOpen, &QToolButton::clicked,
                     this, &MainWindow::onOpen);

    // Separator line
    m_ToolBar->addSeparator();

    // Save
    m_ButtonFileSave = new QToolButton(m_ToolBar);
    m_ButtonFileSave->setIcon(QIcon(":/buttons/document-save.png"));
    m_ButtonFileSave->setText("Save");
    m_ButtonFileSave->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_ToolBar->addWidget(m_ButtonFileSave);
    QObject::connect(m_ButtonFileSave, &QToolButton::clicked,
                     this, &MainWindow::onSave);

    // Save As
    m_ButtonFileSaveAs = new QToolButton(m_ToolBar);
    m_ButtonFileSaveAs->setIcon(QIcon(":/buttons/document-save-as.png"));
    m_ButtonFileSaveAs->setText("Save As");
    m_ButtonFileSaveAs->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_ToolBar->addWidget(m_ButtonFileSaveAs);
    QObject::connect(m_ButtonFileSaveAs, &QToolButton::clicked,
                     this, &MainWindow::onSaveAs);

    // Separator line
    m_ToolBar->addSeparator();

    // Spreadsheet row buffer size
    QToolButton *labelRowBufferSize = new QToolButton(m_ToolBar);
    labelRowBufferSize->setIcon(QIcon(":/buttons/row-buffer.png"));
    labelRowBufferSize->setText("Row Buffer:");
    labelRowBufferSize->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_ToolBar->addWidget(labelRowBufferSize);

    m_SpinBoxRowReadBufferSize = new QSpinBox(m_ToolBar);
    m_SpinBoxRowReadBufferSize->setRange(100, 1000000);
    m_SpinBoxRowReadBufferSize->setValue(Spreadsheet::readBufferSize());
    m_ToolBar->addWidget(m_SpinBoxRowReadBufferSize);
    QObject::connect(m_SpinBoxRowReadBufferSize, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     this, &MainWindow::onReadBufferChange);

    // Separator line
    m_ToolBar->addSeparator();
}

// --- Create the window user interface --- //
void MainWindow::createWindow()
{
    // Window minimum dimensions
    this->setMinimumSize(1152, 648);

    // Main window user interface
    m_TabWidget = new QTabWidget(ui->centralWidget);

    // Set the layout of the main window
    QGridLayout *layout = new QGridLayout(ui->centralWidget);
    layout->addWidget(m_TabWidget);
    ui->centralWidget->setLayout(layout);

    // Introductory text
    m_IntroLabel = new QTextEdit(m_TabWidget);
    m_TabWidget->addTab(m_IntroLabel, "User Guide");
    m_IntroLabel->setReadOnly(true);
    m_IntroLabel->setContentsMargins(10,10,10,10);

    // CSS
    m_IntroLabel->document()->setDefaultStyleSheet(
                QStringLiteral("p { font-size: 10px; }"
                               "h1 { clear: both; }"
                               "h3 { clear: both; color: #4e4e4e; font-style: italic; }"
                               "li { margin-bottom: 10px; }"));

    // About
    m_IntroLabel->insertHtml(QStringLiteral("<h3>Credits</h3>"
                                            "<p>Written by: <b>Archibalduk</b> - <a href=\"http://www.ehmtheblueline.com\">"
                                            "www.ehmtheblueline.com</a></p>"
                                            "<p>Testing by: <b>Dermotron</b> - <a href=\"http://champman0102.co.uk\">"
                                            "www.champman0102.co.uk</a></p>"
                                            "<h3>Icons</h3>"
                                            "<p>All icons are designed by <a href=\"http://www.icons8.com\">"
                                            "www.icons8.com</a> and are used under the Creative Commons Attribution-NoDerivs"
                                            "3.0 Unported licence.</p>"
                                            "<h3>Licence</h3>"
                                            "<p>This application is licensed under the GNU General Public License Version 3 (GPLv3). "
                                            "In summary, this means that this is a free and open source program. You may use the source "
                                            "code from this program in your own programs provided you also release your program "
                                            "under the GPLv3 (which means that your program must also be free and open source). "));

    m_IntroLabel->insertHtml(QString("I used Qt %1 and Microsoft Visual C++ %2 to develop and build this program.</p><br/><br/><br/>")
                             .arg(QT_VERSION_STR).arg(_MSC_VER));

    QFile file("readme.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        m_IntroLabel->insertHtml(stream.readAll());
    }
    else
        m_IntroLabel->insertHtml("<p><b>Click on Open to load a database<b></p></html>");

    // Scroll to the top of the introductory text area
    m_IntroLabel->moveCursor(QTextCursor::Start);

    file.close();
}

// --- Add and show the various tabs --- //
void MainWindow::showTabs()
{
    // Transfer importer
    TransferImporter *transferImporter = new TransferImporter(m_Settings,
                                                              TRANSFER_IMPORTER_SETTINGS_START_POS,
                                                              IMPORT_PATH,
                                                              m_TabWidget);
    m_TabWidget->addTab(transferImporter, "Transfer Import");
    m_TabWidget->setCurrentIndex(1);    // Display the Transfer Import tab

    // Index table
    IndexModel *indexModel = new IndexModel(Index::db);
    m_IndexTable = new QTableView(m_TabWidget);
    m_IndexTable->setModel(indexModel);
    m_TabWidget->addTab(m_IndexTable, "Index");

    // Data exporter
    Exporter *exporter = new Exporter(this);
    this->createExporter(exporter);
    m_TabWidget->addTab(exporter, "Data Export");
}
