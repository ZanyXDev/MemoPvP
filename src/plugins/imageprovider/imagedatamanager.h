#pragma once

#include <QObject>
#include <QtQml>
#include <QDebug>
#include <QStringListModel>

class ImageDataManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QStringListModel* packNamesModel READ packNamesModel CONSTANT)

public:
    explicit ImageDataManager(QObject* parent = nullptr);
    ~ImageDataManager() override = default;

    QStringListModel *packNamesModel() const;

    const QStringList &images() const { return m_images;};
    QString imageFileName(const QString &pack, int pictureId);
    bool isPackExist (const QString &pack) const;
    int imagesInPack  (const QString &pack) const;
    void initModel();

private:
    QStringListModel *m_packNamesModel = nullptr;
    QStringList m_images;
    QStringList m_packs;
};