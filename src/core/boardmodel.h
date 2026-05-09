#pragma once
#include <QAbstractListModel>
#include <QHash>
#include <QByteArray>
#include <QVector>
#include "cellitem.h"

class BoardModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(BoardModel)

public:
    enum BoardRoles {
        PictureIDRole = Qt::UserRole + 1,
        FlippedRole,
        HiddenRole
    };
    Q_ENUM(BoardRoles)

    explicit BoardModel(QObject* parent = nullptr);
    ~BoardModel() override = default;
    // QAbstractItemModel interface
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;

public slots:
    void clear();
    void addCells(const QVector<int>&cells);
private:
    QVector<CellItem> m_cells;
};