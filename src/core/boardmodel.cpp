#include "boardmodel.h"


BoardModel::BoardModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

QHash<int, QByteArray> BoardModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[PictureIDRole] = "pictureID";
    roles[FlippedRole] ="flipped";
    roles[HiddenRole]="hidden";
    return roles;
}

Qt::ItemFlags BoardModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

int BoardModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_cells.size();
}

int BoardModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : this->roleNames().count();
}

QVariant BoardModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_cells.size())
        return QVariant();

    const CellItem& cellItem = m_cells[index.row()];

    switch(role) {
    case PictureIDRole:
        return cellItem.m_pictureID;
    case FlippedRole:
        return cellItem.m_flipped;
    case HiddenRole:
        return cellItem.m_hidden;
    default:
        return QVariant();
    }
}

bool BoardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_cells.size())
        return false;

    bool isOkState;
    CellItem& cellItem = m_cells[index.row()];
    switch(role) {
    case PictureIDRole:
        isOkState = value.canConvert<int>();
        if (isOkState)
            cellItem.m_pictureID = value.toInt();
        break;
    case FlippedRole:
        isOkState =  value.canConvert<bool>();
        if (isOkState)
            cellItem.m_flipped = value.toBool();
        break;
    case HiddenRole:
        isOkState =  value.canConvert<bool>();
        if (isOkState)
            cellItem.m_hidden = value.toBool();
        break;
    default:
        isOkState = false;
    }

    /// TODO Always dataChanged first column in row. is is valid ???
    if (isOkState) emit dataChanged(index, index);

    return isOkState;
}

QModelIndex BoardModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) return QModelIndex();
    return createIndex(row, column);
}

QModelIndex BoardModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex(); // Плоская структура данных
}

void BoardModel::clear()
{
    beginResetModel();
    m_cells.clear();
    endResetModel();
}

/**
 * @brief BoardModel::addCells
 * @param cells QVector<int> cells = {10, 20, 30};
 * @note calls addCells(data);
 */

void BoardModel::addCells(const QVector<int>&cells)
{

    for (int pictureID : cells) {

        CellItem cellItem;
        beginInsertRows(QModelIndex(), m_cells.size(), m_cells.size());

        cellItem.m_pictureID = pictureID;
        cellItem.m_flipped   = false;
        cellItem.m_hidden    = false;

        m_cells.append(cellItem);
        endInsertRows();
    }
}

