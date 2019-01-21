#ifndef EXPORTER_ITEM_H
#define EXPORTER_ITEM_H

#include <QString>

#include "../spreadsheet/spreadsheet.h"

// --- Exporter item --- //
class ExporterItem
{
private:
    // Id
    unsigned short m_Id;

    // Exporter data
    QString m_Title;
    QString m_Description;
    bool    (*m_ExportSpreadsheet)(Spreadsheet &);

public:
    // Constructor
    ExporterItem();
    ExporterItem(bool (exportFunction)(Spreadsheet &),
                 QString title,
                 QString description);

    // Get data
    QString description();
    bool exportSpreadsheet(Spreadsheet &s);
    unsigned short id();
    QString title();

    // Set data
    void setId(unsigned short id);
};

#endif // EXPORTER_ITEM_H
