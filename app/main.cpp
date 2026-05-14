#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QLibraryInfo>
#include <QVariant>
#include <QVariantMap>

#ifdef QT_DEBUG
#include <QtCore/QLoggingCategory>
#include <QDir>
#include <QQuickWindow>
#endif

#ifdef QT_DEBUG
static void scanDirectoryPaths(const QString& path, QStringList& paths) {
    QDir dir(path);

    for (const QString& entry : dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        QString fullPath = path + "/" + entry;
        paths.append(fullPath);

        QFileInfo info(fullPath);
        if (info.isDir()) {
            scanDirectoryPaths(fullPath, paths);
        }
    }
}
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(PACKAGE_NAME_STR);
    QCoreApplication::setApplicationName(ACTIVITY_NAME_STR);
    QCoreApplication::setApplicationVersion(VERSION_STR);

#ifdef QT_DEBUG
    qputenv("QT_LOGGING_RULES",
            "qt.qml.binding.removal.info=true;"
            "qt.qml.imports.debug=true;");
    QLoggingCategory::setFilterRules(qgetenv("QT_LOGGING_RULES"));
// "qt.scenegraph.time.renderer=true;"
// "qt.scenegraph.time.renderloop=true;");
#endif

    QGuiApplication app(argc, argv);

#ifdef Q_OS_ANDROID
    QNativeInterface::QAndroidApplication::hideSplashScreen(333);
#endif

    QQmlApplicationEngine engine;

    //  КРИТИЧНО: Добавляем пути поиска модулей для разработки
    //  Стандартные пути установки Qt
    engine.addImportPath(QLibraryInfo::path(QLibraryInfo::QmlImportsPath));
#ifdef QT_DEBUG
    QFile vertCheck("://qt/qml/assets/shaders/flip.vert.qsb");
    QFile fragCheck("://qt/qml/assets/shaders/flip.frag.qsb");

    qInfo() << "Vertex shader exists:" << vertCheck.exists();
    qInfo() << "Fragment shader exists:" << fragCheck.exists();
    qInfo() << "Resource list sample:" << QDir(":/").entryList();

    QStringList paths;
    scanDirectoryPaths("://qt/qml", paths);
    qDebug() << "[DEV.Core] All resources:" << paths;

#endif

#ifdef QT_DEBUG
#ifdef QML_DEV_IMPORT_PATH
    // Путь к модулям в build-директории (для локальной разработки)
    // Используем applicationDirPath() — где лежит исполняемый файл
    const QString buildImportPath = QString::fromUtf8(QML_DEV_IMPORT_PATH);
    qDebug() << "[APP.CPP]  buildImportPath (from CMake):" << buildImportPath;
    if (QDir(buildImportPath).exists()) {
        engine.addImportPath(buildImportPath);
        qDebug() << "[APP.CPP] Added QML import path:" << buildImportPath;
    } else {
        qWarning() << "[APP.CPP] WARNING: CMake QML path does not exist:" << buildImportPath;
    }
#endif
    qDebug() << "[APP.CPP] QML import paths:" << engine.importPathList();
#endif

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    /**
     * @brief В то время как синглтоны могут использоваться для передачи состояния на QML, они расточительны,
     * когда состояние необходимо только для первоначальной настройки приложения.
     * В этом случае часто можно использовать QQmlApplicationEngine:setInitialProperties.
     */
    bool isMobile = []() {
#ifdef Q_OS_ANDROID
        return true;
#else
        return false;
#endif
    }();

    bool isDebugMode = []() {
#ifdef QT_DEBUG
        return true;
#else
        return false;
#endif
    }();

    QString appVersion = QCoreApplication::applicationVersion();
    QVariantMap initialProperties;
    initialProperties["isMobile"] = QVariant::fromValue(isMobile);
    initialProperties["isDebugMode"] = QVariant::fromValue(isDebugMode);
    initialProperties["appVersion"] = QVariant::fromValue(appVersion);

    engine.setInitialProperties(initialProperties);
    engine.loadFromModule("io.github.zanyxdev.memopvp", "Main");

    return QCoreApplication::exec();
}
