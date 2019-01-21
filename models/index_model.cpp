#include "index_model.h"

/* ===================== */
/*      Index Model      */
/* ===================== */

// --- Constructor --- //
IndexModel::IndexModel(QVector<Index> &data) :
    m_Data(&data)
{

}


/* ================ */
/*      Counts      */
/* ================ */

// --- Column count --- //
int IndexModel::columnCount(const QModelIndex & /*parent*/) const
{
    return COLUMN_COUNT;
}

// --- Row/QVector count --- //
int IndexModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_Data->size();
}


/* ============== */
/*      Data      */
/* ============== */

// --- Get data --- //
QVariant IndexModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        Index *i = &(*m_Data)[index.row()];

        switch(index.column()) {

        case FILE_NAME:
            return i->getFileName();

        case FILE_ID:
            return i->FileId;

        case TABLE_SIZE:
            return i->TableSize;

        case OFFSET:
            return i->Offset;

        case VERSION:
            return i->Version;
        }
    }

    return QVariant();
}


/* ================ */
/*      Header      */
/* ================ */

// --- Get header --- //
QVariant IndexModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // Horizontal header
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section) {

        case FILE_NAME:
            return "Filename";

        case FILE_ID:
            return "ID";

        case TABLE_SIZE:
            return "Records";

        case OFFSET:
            return "Offest";

        case VERSION:
            return "Version";
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
Qt::ItemFlags IndexModel::flags(const QModelIndex &index) const
{
    // Read only
    return QAbstractTableModel::flags(index);
}
