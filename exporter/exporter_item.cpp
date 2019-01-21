#include "exporter_item.h"

/* ======================= */
/*      Exporter Item      */
/* ======================= */

// --- Default constructor --- //
ExporterItem::ExporterItem() :
    m_ExportSpreadsheet(nullptr)
{

}

// --- Actual constructor --- //
ExporterItem::ExporterItem(bool (exportFunction)(Spreadsheet &), QString title, QString description) :
    m_ExportSpreadsheet(exportFunction),
    m_Title(title),
    m_Description(description)
{

}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Get description --- //
QString ExporterItem::description()
{
    return m_Description;
}

// --- Export the spreadsheet via the function pointer --- //
bool ExporterItem::exportSpreadsheet(Spreadsheet &s)
{
    if(m_ExportSpreadsheet == nullptr)
        return false;
    else
        return m_ExportSpreadsheet(s);
}

// --- Get id --- //
unsigned short ExporterItem::id()
{
    return m_Id;
}

// --- Get title --- //
QString ExporterItem::title()
{
    return m_Title;
}


/* ================== */
/*      Set Data      */
/* ================== */

// --- Set id --- //
void ExporterItem::setId(unsigned short id)
{
    m_Id = id;
}
