#pragma once

#include <QImage>
#include <QPainter>
#include <QQuickImageProvider>


class ImagePackProvider : public QQuickImageProvider
{
public:
    ImagePackProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
    const QStringList& packNames() const{ return m_packs; };  // Константная ссылка

private:
    QStringList m_images;
    QStringList m_packs;
    void findImagesInPacks();
};
