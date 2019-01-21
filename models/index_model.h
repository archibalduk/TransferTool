#ifndef INDEX_MODEL_H
#define INDEX_MODEL_H

// Qt headers
#include <QAbstractTableModel>
#include <QVector>

// Recorder headers
#include "../database/index.h"

// --- Index Model --- //
class IndexModel : public QAbstractTableModel
{
private:
    // Pointer to underlying data
    QVector<Index> *m_Data;

public:
    // Constructor
    IndexModel(QVector<Index> &data);

    // Counts
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    // Data
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    // Header
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    // Flags
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    // Column numbers
    enum ENUM_COLUMNS {
        FILE_NAME = 0,
        FILE_ID,
        TABLE_SIZE,
        OFFSET,
        VERSION,
        COLUMN_COUNT
    };
};

#endif // INDEX_MODEL_H
