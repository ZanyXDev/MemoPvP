#include <QDir>
#include <QDirListing>
#include <QDirIterator>
#include <QStringList>

#include "imagedatamanager.h"


ImageDataManager *ImageDataManager::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)

    if (s_instance.loadAcquire() == nullptr) {
        auto* instance = new ImageDataManager();
        s_instance.storeRelease(instance);
        return instance;
    }

    // Если уже создан, возвращаем существующий (QML этого не ожидает!)
    qWarning() << "ImageDataManager already exists - returning existing instance";
    return s_instance.loadAcquire();
}

QString ImageDataManager::getRealFileName(const QString &id)
{
    if ( m_images.isEmpty() ) {
        qWarning() <<Q_FUNC_INFO << " No images found in resources!";
        return QString();
    }

    // Валидация входного параметра
    if (id.isEmpty()) {
        qWarning() << Q_FUNC_INFO <<  "Empty id requested !";
        return QString();
    }

    QString packName = getPackNameFromPatch( id );

    if (packName.isEmpty()){
        // обычное изображение возвращаем путь

        qDebug() <<"id:"<<id <<" m_images.filter( id ).count():" << m_images.filter( id ).count();

        if (m_images.filter( id ).count() > 0 ){
            return m_images.filter(id).at(0);
        }
    }else{
        // нашли изображение pack
        QStringList parts = id.split("/");
        if (parts.size() < 2) {
            qWarning() << "[ImagePackProvider] Invalid id format:" << id << "- expected format: '__packName/index'";
            return QString();
        }
        bool conversionOk = false;
        int pictureId = parts[1].toInt(&conversionOk);
        if (conversionOk){
            if (pictureId >=0 && pictureId < m_packs.filter("__"+packName ).count() ){
                return m_packs.filter("__"+packName ).at( pictureId );
            }
        }else{
            qWarning() << "[ImagePackProvider] Invalid id format (index conversion):" << id << "- expected format: '__packName/index'";
            return QString();
        }
    }
    return QString();
}

QStringListModel *ImageDataManager::packNamesModel() const
{
    return m_packNamesModel;
}


QString ImageDataManager::currentPackName() const
{
    return m_currentPackName;
}


int ImageDataManager::currentPackImagesCount() const
{
    return m_currentPackImagesCount;
}

void ImageDataManager::updateCurrentPackData(int current)
{
    if (current == -1 ) return;

    if ( current <= m_packsName.count()){
        QString packNameModel = m_packsName.at( current );
        if ( packNameModel != m_currentPackName ){
/// ToDO Что будет если произошла рассинхронизация m_packNamesModel и m_packsName
                m_currentPackName = packNameModel;
                m_currentPackImagesCount = m_packs.filter("__"+packNameModel ).count();
                emit currentPackNameChanged();
                emit currentPackImagesCountChanged();

        }
    }
}

// ------------------------ private -----------------------------------------
ImageDataManager::ImageDataManager(QObject *parent)
    : QObject(parent)
    , m_packNamesModel(new QStringListModel( this) )
{
    qDebug() << "[INIT_ORDER] >>> ImageDataManager created at"
             << QTime::currentTime().toString("hh:mm:ss.zzz")
             << ", instance:" << this;

    getAllImagesFileName();
}

void ImageDataManager::getAllImagesFileName()
{
    m_images.clear();
    m_packs.clear();
    m_packsName.clear();

    // Определяем список расширений изображений
    const QStringList imageFilters = { "*.png", "*.jpg", "*.jpeg", "*.svg" };

    // Создаём итератор с рекурсивным обходом и фильтрами
    QDirIterator it(":/qt/qml/assets/images",
                    imageFilters, QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();
        const QString filePath = it.filePath();
        QString _packName = getPackNameFromPatch(filePath);
        if (_packName.isEmpty()){
            m_images.append( filePath );
        }else{
            if ( !m_packsName.contains( _packName,Qt::CaseInsensitive )){
                m_packsName.append( _packName );               
            }
            m_packs.append( filePath );
        }
    }

    if ( m_images.isEmpty() || m_packs.isEmpty() || m_packsName.empty()) {
        qCritical() << "[DEV.plugin] ImageDataManager: No images found in resources!";
        Q_ASSERT_X(false, "ImageDataManager::getAllImagesFileName", "Broken qresources!!");
    }else{
        m_packNamesModel->setStringList( m_packsName );
        updateCurrentPackData( 0 );
    }
}

QString ImageDataManager::getPackNameFromPatch(const QString &path)
{
    /**
     * @brief re Поиск названия бандла с картинками
     * .*? чтобы поиск остановился сразу на первом встреченном __:
     *  __ — два символа подчёркивания.
     * ( начало захвата группы
     *  [^/]+ — один или более любых символов, кроме /.
     *  ) конец захвата группы
     *  / — обязательный слэш после текста.
     */
    QRegularExpression re(".*?__([^/]+)/.*");

    QRegularExpressionMatch match = re.match( path );
    if (match.hasMatch()) {
        return match.captured(1); // Вернет "animals" или "emoji"
    } else {
        qWarning() << "Совпадение не найдено для пути:" << path;
        return QString();
    }
}

