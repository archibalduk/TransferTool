#ifndef EXPORTER_H
#define EXPORTER_H

// Qt headers
#include <QDataWidgetMapper>
#include <QGridLayout>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QVector>
#include <QWidget>

// Exporter headers
#include "exporter_item.h"
#include "exporter_model.h"

// Other headers
#include "../spreadsheet/spreadsheet.h"

// --- Exporter interface --- //
class Exporter : public QWidget
{
private:
    // Underlying data
    QVector<ExporterItem*> m_ExporterItems;

    // Vector position counter
    static unsigned short s_PositionCounter;

    // Model data
    QDataWidgetMapper *m_Mapper;
    ExporterModel *m_Model;

    // User interface
    QPushButton *m_Button;
    QLineEdit *m_Description;
    QLineEdit *m_Id;
    QLineEdit *m_Title;
    QListView *m_ListView;

private slots:
    // Export
    void onClick();

    // Model
    void onModelChanged(const QModelIndex &index);

public:
    // Constructor
    Exporter(QWidget *parent = 0);

    // Add data
    void add(bool (exportFunction)(Spreadsheet &),
             QString title,
             QString description);
};

#endif // EXPORTER_H
