#include "exporter.h"
#include "../progress_window.h"
#include <QMessageBox>

// --- Static data --- //
unsigned short Exporter::s_PositionCounter = 0;


/* ============================ */
/*      Exporter Interface      */
/* ============================ */

// --- Default constructor --- //
Exporter::Exporter(QWidget *parent) :
    QWidget(parent)
{
    // Model
    m_Model = new ExporterModel(m_ExporterItems);

    // Data widget mapper
    m_Mapper = new QDataWidgetMapper(this);
    m_Mapper->setModel(m_Model);

    // ListView
    m_ListView = new QListView(this);
    m_ListView->setUniformItemSizes(true);  // As only uniform sizes are used, the ListView can be optimised using setUniformItemSizes()
    m_ListView->setModel(m_Model);
    m_ListView->setModelColumn(ExporterModel::TITLE);

    // Connect the widget mapper to the list view
    QObject::connect(m_ListView, &QListView::clicked,
                     this, &Exporter::onModelChanged);

    // Title
    m_Title = new QLineEdit("Title", this);
    m_Title->setReadOnly(true);
    m_Mapper->addMapping(m_Title, ExporterModel::TITLE);

    // Description
    m_Description = new QLineEdit("Description", this);
    m_Description->setReadOnly(true);
    m_Mapper->addMapping(m_Description, ExporterModel::DESCRIPTION);

    // Id
    m_Id = new QLineEdit("ID", this);
    m_Id->setReadOnly(true);
    m_Mapper->addMapping(m_Id, ExporterModel::ID);

    // Button
    m_Button = new QPushButton("Export Data", this);
    QObject::connect(m_Button, &QPushButton::clicked,
                     this, &Exporter::onClick);

    // Layout
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_ListView, 0, 0, 3, 1);
    layout->addWidget(m_Id, 0, 1);
    layout->addWidget(m_Title, 0, 2);
    layout->addWidget(m_Description, 1, 1, 1, 2);
    layout->addWidget(m_Button, 2, 1, 1, 2);
    this->setLayout(layout);
}


/* ================== */
/*      Add Data      */
/* ================== */

// --- Add ExporterItem --- //
void Exporter::add(bool exportFunction(Spreadsheet &), QString title, QString description)
{
    // New ExporterItem
    ExporterItem *item = new ExporterItem(exportFunction, title, description);

    // Position counter
    item->setId(s_PositionCounter);
    ++s_PositionCounter;

    // Add to QVector
    m_ExporterItems.push_back(item);

    // Move to first index (this is a bit of a hack)
    m_Mapper->toFirst();
}


/* ===================== */
/*      Export Data      */
/* ===================== */

// --- Export data --- //
void Exporter::onClick()
{
    // Vector position/id
    unsigned short id = m_Id->text().toShort();

    // Abort if the selected item is out of range
    if(id < 0 || id >= m_ExporterItems.size())
        return;

    Spreadsheet s;

    // Show open dialog and abort if the user cancels
    if(!s.create(QString(), m_Title->text()))
        return;

    ProgressWindow *p = new ProgressWindow("Preparing data", 2);
    p->increment();

    // Write the file if export function pointer returned true
    if(m_ExporterItems[id]->exportSpreadsheet(s)) {
        p->complete();
        delete p;
        s.write();
    }
    else {
        p->complete();
        delete p;
        QMessageBox::critical(0, "Export Error", "An error was encountered when attempting to export the data.");
    }
}


/* =============== */
/*      Model      */
/* =============== */

// --- Model --> mapper slot --- //
void Exporter::onModelChanged(const QModelIndex &index)
{
    m_Mapper->setCurrentModelIndex(index);
}
