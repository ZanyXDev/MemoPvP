#pragma once

#include <QImage>
#include <QPainter>
#include <QQuickImageProvider>

class ImageDataManager;

class ImagePackProvider : public QQuickImageProvider
{
public:
    ImagePackProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    ImageDataManager *m_imageDataManager = nullptr;
    QStringList m_images;
    QStringList m_packs;
    void findImagesInPacks();
};
