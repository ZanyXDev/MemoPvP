#include <QDirListing>
#include <QDir>
#include <QDirIterator>

#include "imagedatamanager.h"

ImageDataManager::ImageDataManager(QObject *parent)
    : QObject(parent)
    , m_packNamesModel(new QStringListModel( this) )
{
    qDebug() << "[DEV.plugin] ImageDataManager" <<  Q_FUNC_INFO << this;

}

QStringListModel *ImageDataManager::packNamesModel() const
{
    return m_packNamesModel;
}

QString ImageDataManager::imageFileName(const QString &pack, int pictureId)
{
    if ( !isPackExist(pack) ) return QString();
    if ( pictureId >= imagesInPack(pack) ) {
        qWarning() << "[DEV.plugin] ImageDataManager Index out of range. Requested:" << pictureId
                   << ", available images in pack" << pack << ":" << imagesInPack(pack);
        return QString();
    }
    return  m_images.filter(pack + "/").at( pictureId);
}

bool ImageDataManager::isPackExist(const QString &pack) const
{
#ifdef QT_DEBUG
    if (!m_packs.contains(pack)) {
        qWarning() << "[DEV.plugin] ImageDataManager Pack not found:" << pack << "- available packs:" << m_packs;
    }
#endif
    return m_packs.contains( pack );
}

int ImageDataManager::imagesInPack(const QString &pack) const
{
    if ( isPackExist(pack) ){
        return  m_images.filter(pack + "/").count();
    }
    return -1;
}

void ImageDataManager::initModel()
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
    ///ToDo check m_packNamesModel setStringList(m_packs) don't append
    m_packNamesModel->setStringList(m_packs);
}
