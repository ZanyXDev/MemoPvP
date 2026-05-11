#include <QPainter>
#include <QColor>
#include <QImage>
#include <QDirListing>
#include <QDir>
#include <QDirIterator>


#include "imagepackprovider.h"

ImagePackProvider::ImagePackProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{    
    this->findImagesInPacks();
}

QPixmap ImagePackProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
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
    // Проверка существования пака
    if (!m_packs.contains(packName)) {
        qWarning() << "[ImagePackProvider] Pack not found:" << packName << "- available packs:" << m_packs;
        return QPixmap();
    }
    // Поиск изображения по пакету
    QStringList filtered = m_images.filter(packName + "/");
    if (pictureId >= filtered.size()) {
        qWarning() << "[ImagePackProvider] Index out of range. Requested:" << pictureId
                   << ", available images in pack" << packName << ":" << filtered.size();
        return QPixmap();
    }

    QString filePath = filtered.at(pictureId);

    QPixmap pixmap;
    if (!pixmap.load(filePath)) {
        qWarning() << "[ImagePackProvider] Failed to load image:" << filePath;
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


void ImagePackProvider::findImagesInPacks()
{    
    m_packs.clear();
    m_images.clear();
    using F = QDirListing::IteratorFlag;
    const auto flags = F::FilesOnly | F::Recursive;

    for (const auto &dirEntry : QDirListing(":/assets/images/packs", flags)) {
        // image filepath ":/assets/images/packs/emoji/emoji_0.png"
        m_images.append( dirEntry.filePath() );
        QStringList parts = dirEntry.filePath().split("/");
        if ( !parts[4].isEmpty() && !m_packs.contains( parts[4],Qt::CaseInsensitive )){
            m_packs.append( parts[4] );
        }
    }
    qDebug() << "[DEV]  m_packs:" << m_packs;
}
