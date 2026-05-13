#include <QDirListing>
#include <QDir>
#include <QDirIterator>

#include "imagedatamanager.h"

ImageDataManager::ImageDataManager(QObject *parent)
    : QObject(parent)
    , m_packNamesModel(new QStringListModel( this) )
{
    qDebug() << "[INIT_ORDER] >>> ImageDataManager created at"
             << QTime::currentTime().toString("hh:mm:ss.zzz")
             << ", instance:" << this;

    // ✅ Сохраняем указатель атомарно (потокобезопасно)
    s_instance.storeRelease(this);
    this->initModel();
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
    qDebug() << "[DEV.plugin] ImageDataManager::isPackExist find pack" << pack << "- available packs:" << m_packs;
    if (  m_packs.isEmpty()) {
        qCritical() << "[DEV.plugin] ImageDataManager:isPackExist No image packs found in resources!";
        Q_ASSERT_X(false, "ImageDataManager::initModel", "Broken qresources!!");
        return false;
    }
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

    for (const auto &dirEntry : QDirListing(":/qt/qml/assets/images/packs", flags)) {
        // "://qt/qml/assets/images/packs/animals/animals_0.png",
#ifdef QT_DEBUG
        qDebug() << "dirEntry.filePath():" <<dirEntry.filePath();
        qDebug() << "dirEntry.filePath().split(\"/\")" << dirEntry.filePath().split("/");
#endif
        m_images.append( dirEntry.filePath() );
        QStringList parts = dirEntry.filePath().split("/");

        if ( !parts[6].isEmpty() && !m_packs.contains( parts[6],Qt::CaseInsensitive )){
            m_packs.append( parts[6] );
        }
    }
    if ( ! m_packs.isEmpty()) {
        m_packNamesModel->setStringList(m_packs);
        setCurrentPackName(m_packs.first());
    }else{
        qCritical() << "[DEV.plugin] ImageDataManager: No image packs found in resources!";
        Q_ASSERT_X(false, "ImageDataManager::initModel", "Broken qresources!!");
    }
}

QString ImageDataManager::currentPackName() const
{
    qDebug() << Q_FUNC_INFO << "this" << this
             << "m_currentPackName:" << m_currentPackName
             << "m_packs.count:" << m_packs.count();
    return m_currentPackName;
}

void ImageDataManager::setCurrentPackName(const QString &newCurrentPackName)
{
    qDebug() << Q_FUNC_INFO
             << "m_currentPackName:" << m_currentPackName
             << "newCurrentPackName:" << newCurrentPackName;

    if (m_currentPackName == newCurrentPackName)
        return;
    if ( isPackExist(newCurrentPackName) ){

        m_currentPackName = newCurrentPackName;
        qDebug() << Q_FUNC_INFO << "Pack exist."
                 << "m_currentPackName:" << m_currentPackName;

        emit currentPackNameChanged();
        m_currentPackImagesCount = imagesInPack(m_currentPackName);
        emit currentPackImagesCountChanged();
    }
}

int ImageDataManager::currentPackImagesCount() const
{
    return m_currentPackImagesCount;
}



