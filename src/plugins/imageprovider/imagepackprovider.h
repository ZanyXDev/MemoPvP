#pragma once

#include <QImage>
#include <QPainter>
#include <QQuickImageProvider>


class ImagePackProvider : public QQuickImageProvider
{
public:
    ImagePackProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
private:
    QStringList m_imagesPackList;
    void findImagesInPacks();
};
