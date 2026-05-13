//src/plugins/imageprovider/imagepackprovider.cpp
#include "imagepackprovider.h"
#include "imagedatamanager.h"

ImagePackProvider::ImagePackProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    qDebug().nospace() << "[INIT_ORDER] >>> ImagePackProvider created at "
                       << QTime::currentTime().toString("hh:mm:ss.zzz")
                       << ", instance:" << static_cast<void*>(this);
}

QPixmap ImagePackProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    // ✅ Мгновенный и потокобезопасный доступ к синглтону
    auto* dataManager = ImageDataManager::instance();
    if (!dataManager) {
        qWarning() << "[ImagePackProvider] ImageDataManager not initialized yet!";
        return QPixmap();
    }

    // Валидация входного параметра
    if (id.isEmpty()) {
        qWarning() << "[ImagePackProvider] Empty id requested";
        return QPixmap();
    }

    // Разбор ID: "animals/1" -> ["animals", "1"]
    QStringList parts = id.split("/");
    if (parts.size() < 2) {
        qWarning() << "[ImagePackProvider] Invalid id format:" << id << "- expected format: 'packName/index'";
        return QPixmap();
    }
    QString packName = parts[0];
    bool conversionOk = false;
    int pictureId = parts[1].toInt(&conversionOk);

    if (!conversionOk || pictureId < 0) {
        qWarning() << "[ImagePackProvider] Invalid index in id:" << id << "- index must be non-negative integer";
        return QPixmap();
    }


    // ✅ Вызываем метод синглтона
    QString fileName = dataManager->imageFileName(packName, pictureId);

    if (fileName.isEmpty()) return QPixmap();

    QPixmap pixmap;
    if (!pixmap.load(fileName)) {
        qWarning() << "[ImagePackProvider] Failed to load image:" << fileName;
        return QPixmap();
    }
    // Масштабирование, если запрошен другой размер
    if (requestedSize.isValid() && !requestedSize.isEmpty()) {
        if (pixmap.size() != requestedSize) {
            Qt::AspectRatioMode aspectMode = Qt::KeepAspectRatio;
            Qt::TransformationMode transformMode = Qt::SmoothTransformation;

            qDebug() << "[ImagePackProvider] Scaled image from" << pixmap.size()
                     << "to" << requestedSize << "(aspect ratio kept)";
            pixmap = pixmap.scaled(requestedSize, aspectMode, transformMode);

            if (size) {
                *size = pixmap.size();
            }
        } else if (size) {
            *size = pixmap.size();
        }
    } else if (size) {
        *size = pixmap.size();
    }

    return pixmap;
}




