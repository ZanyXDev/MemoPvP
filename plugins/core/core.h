#pragma once
#include <QObject>
#include <QtQml>
#include <QStandardItemModel>

class Core : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QStandardItemModel* boardModel READ boardModel CONSTANT)
    Q_PROPERTY(bool HideCard READ HideCard WRITE setHideCard NOTIFY HideCardChanged FINAL)
public:
    explicit Core(QObject *parent = nullptr);
    QStandardItemModel *boardModel() const;
    bool HideCard() const;
    void setHideCard(bool newHideCard);

signals:
    void HideCardChanged();

private:
    QStandardItemModel *m_boardModel = nullptr;
    bool m_HideCard;
};
