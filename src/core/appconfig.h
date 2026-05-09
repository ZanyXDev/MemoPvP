// appconfig.h
#pragma once
#include <QObject>
#include <QCoreApplication>
#include <QtQml>

///TODO Убрать класс из видимости Qml
///     перести логику взаимодйествия с Qml в GameEngine
class AppConfig : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON  // ← Ключевая метка для Qt6

    Q_PROPERTY(bool isMobile READ isMobile CONSTANT)
    Q_PROPERTY(bool isDebugMode READ isDebugMode CONSTANT)
    Q_PROPERTY(QString appVersion READ appVersion CONSTANT)
    Q_PROPERTY(QString buildQtVersion READ buildQtVersion CONSTANT)
public:
    explicit AppConfig(QObject *parent = nullptr) : QObject(parent) {}
    
    bool isMobile() const;
    bool isDebugMode() const;
    QString appVersion() const;
    QString buildQtVersion() const;
private:
    // Кэшируем значения при инициализации
    bool m_isMobile = []() {
#ifdef Q_OS_ANDROID
        return true;
#else
        return false;
#endif
    }();

    bool m_isDebugMode = []() {
#ifdef QT_DEBUG
        return true;
#else
        return false;
#endif
    }();

    QString m_appVersion = QCoreApplication::applicationVersion();
    QString m_buildQtVersion = QString("This program build with Qt version %1.").arg(QT_VERSION_STR);
};
