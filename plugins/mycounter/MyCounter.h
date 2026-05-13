#pragma once
#include <QObject>
#include <QtQml>

class MyCounter : public QObject {
    Q_OBJECT
    QML_ELEMENT // Qt 6.11: автоматически регистрирует тип и включает в плагин
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged FINAL)

public:
    explicit MyCounter(QObject *parent = nullptr);
    int value() const;
    void setValue(int newValue);

signals:
    void valueChanged();

private:
    int m_value = 0;
};
