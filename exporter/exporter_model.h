#ifndef EXPORTER_MODEL_H
#define EXPORTER_MODEL_H

// Qt headers
#include <QAbstractTableModel>
#include <QVector>

// Exporter headers
#include "exporter_item.h"

// --- Exporter Model --- //
class ExporterModel : public QAbstractTableModel
{
private:
    // Pointer to underlying data
    QVector<ExporterItem*> *m_Data;

public:
    // Constructor
    ExporterModel(QVector<ExporterItem*> &data);

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
        TITLE = 0,
        DESCRIPTION,
        ID,
        COLUMN_COUNT
    };
};

#endif // EXPORTERMODEL_H
