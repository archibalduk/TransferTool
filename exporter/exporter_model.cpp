#include "exporter_model.h"

/* ===================== */
/*      Index Model      */
/* ===================== */

// --- Constructor --- //
ExporterModel::ExporterModel(QVector<ExporterItem*> &data) :
    m_Data(&data)
{

}


/* ================ */
/*      Counts      */
/* ================ */

// --- Column count --- //
int ExporterModel::columnCount(const QModelIndex & /*parent*/) const
{
    return COLUMN_COUNT;
}

// --- Row/QVector count --- //
int ExporterModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_Data->size();
}


/* ============== */
/*      Data      */
/* ============== */

// --- Get data --- //
QVariant ExporterModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        ExporterItem *i = (*m_Data)[index.row()];

        switch(index.column()) {

        case TITLE:
            return i->title();

        case DESCRIPTION:
            return i->description();

        case ID:
            return i->id();
        }
    }

    return QVariant();
}


/* ================ */
/*      Header      */
/* ================ */

// --- Get header --- //
QVariant ExporterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // Horizontal header
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section) {

        case TITLE:
            return "Title";

        case DESCRIPTION:
            return "Description";

        case ID:
            return "ID";
        }
    }

    // Vertical header (i.e. row numbers)
    else if(orientation == Qt::Vertical && role == Qt::DisplayRole)
        return section+1;

    return QVariant();
}


/* =============== */
/*      Flags      */
/* =============== */

// --- Flags --- //
Qt::ItemFlags ExporterModel::flags(const QModelIndex &index) const
{
    // Read only
    return QAbstractTableModel::flags(index);
}
