#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QLibraryInfo>

#ifdef QT_DEBUG
#include <QtCore/QLoggingCategory>
#include <QDir>
#include <QQuickWindow>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(PACKAGE_NAME_STR);
    QCoreApplication::setApplicationName(ACTIVITY_NAME_STR);
    QCoreApplication::setApplicationVersion(VERSION_STR);

#ifdef QT_DEBUG_MY
    qputenv("QT_LOGGING_RULES",
            "qt.qml.binding.removal.info=true;"
            "qt.qml.imports.debug=true;"
            "qt.scenegraph.time.renderer=true;"
            "qt.scenegraph.time.renderloop=true;"
            "qt.rhi=debug;"
            "qt.scenegraph=debug");
    QLoggingCategory::setFilterRules(qgetenv("QT_LOGGING_RULES"));
#endif

    QGuiApplication app(argc, argv);

#ifdef QT_DEBUG
    QFile vertCheck("://qt/qml/shadereffects/assets/shaders/flip.vert.qsb");
    QFile fragCheck("://qt/qml/shadereffects/assets/shaders/flip.frag.qsb");

    qInfo() << "Vertex shader exists:" << vertCheck.exists();
    qInfo() << "Fragment shader exists:" << fragCheck.exists();
    qInfo() << "Resource list sample:" << QDir(":/").entryList();

    qInfo() << "Scene Graph backend:" << QQuickWindow::sceneGraphBackend();

#endif

#ifdef Q_OS_ANDROID
    QNativeInterface::QAndroidApplication::hideSplashScreen(333);
#endif

    QQmlApplicationEngine engine;

// ✅ КРИТИЧНО: Добавляем пути поиска модулей для разработки
// 1. Стандартные пути установки Qt
    engine.addImportPath(QLibraryInfo::path(QLibraryInfo::QmlImportsPath));

// 2. Путь к модулям в build-директории (для локальной разработки)
#ifdef QT_DEBUG
    // Используем applicationDirPath() — где лежит исполняемый файл
    const QString appDir = QCoreApplication::applicationDirPath();
    const QString buildImportPath = appDir + "/qt/qml";
    qDebug() << "[DEV]  buildImportPath:" << buildImportPath;
    if (QDir(buildImportPath).exists()) {
        engine.addImportPath(buildImportPath);
        qDebug() << "[DEV] Added QML import path:" << buildImportPath;
    }
    qDebug() << "[DEV] QML import paths:" << engine.importPathList();
#endif

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("io.github.zanyxdev.memopvp", "Main");


    return QCoreApplication::exec();
}
