#include "core.h"
#include <QDebug>

Core::Core(QObject *parent)
    : QObject(parent)
    , m_boardModel (new QStandardItemModel( this) )
{
    qDebug() << "[INIT_ORDER] >>> CorePlugin created at"
             << QTime::currentTime().toString("hh:mm:ss.zzz")
             << ", instance:" << this;
    QHash<int, QByteArray> roleNames = m_boardModel->roleNames();
    roleNames[Qt::UserRole + 1] = "pictureId";
    roleNames[Qt::UserRole + 2] = "cardVisible";
    m_boardModel->setItemRoleNames(roleNames);
#ifdef QT_DEBUG
    m_boardModel->setColumnCount(1);
    const int N = 16;
    m_boardModel->setRowCount(N);
    for (int r = 0; r < N; ++r)
    {
        QStandardItem* item = new QStandardItem();

        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setData( r ,Qt::UserRole +1);
        item->setData(Qt::Checked, Qt::UserRole +2);

        m_boardModel->setItem(r, 0, item);
    }
#endif
}


QStandardItemModel *Core::boardModel() const
{
    return m_boardModel;
}

bool Core::HideCard() const
{
    return m_HideCard;
}

void Core::setHideCard(bool newHideCard)
{
    if (m_HideCard == newHideCard)
        return;
    m_HideCard = newHideCard;
    QModelIndex idx = m_boardModel->index(0,0);
    m_boardModel->setData(idx,Qt::Unchecked, Qt::UserRole +2);
    emit HideCardChanged();
}
