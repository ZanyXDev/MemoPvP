#pragma once

#include <QObject>
#include <QDebug>
#include <QStringListModel>
#include <QtQml>

#include <QAtomicPointer> // ✅ Для потокобезопасного доступа из QQuickImageProvider

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

    // ✅ Потокобезопасный доступ к экземпляру синглтона
    static ImageDataManager* instance() { return s_instance.loadAcquire(); }

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
    // ✅ Атомарный указатель: безопасно читать из любого потока
    inline static QAtomicPointer<ImageDataManager> s_instance{nullptr};

    QStringListModel *m_packNamesModel = nullptr;
    QStringList m_images;
    QStringList m_packs;
    QString m_currentPackName;
    int m_currentPackImagesCount;
};