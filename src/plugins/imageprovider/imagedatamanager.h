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
    Q_PROPERTY(QString currentPackName READ currentPackName WRITE setCurrentPackName NOTIFY currentPackNameChanged FINAL)
    Q_PROPERTY(int currentPackImagesCount READ currentPackImagesCount NOTIFY currentPackImagesCountChanged FINAL)
public:
    explicit ImageDataManager(QObject* parent = nullptr);
    ~ImageDataManager() override = default;

    QStringListModel *packNamesModel() const;
    QString imageFileName(const QString &pack, int pictureId);
    bool isPackExist (const QString &pack) const;
    int imagesInPack  (const QString &pack) const;
    void initModel();

    QString currentPackName() const;
    void setCurrentPackName(const QString &newCurrentPackName);

    int currentPackImagesCount() const;

signals:
    void currentPackNameChanged();

    void currentPackImagesCountChanged();

private:
    QStringListModel *m_packNamesModel = nullptr;
    QStringList m_images;
    QStringList m_packs;
    QString m_currentPackName;
    int m_currentPackImagesCount;
};