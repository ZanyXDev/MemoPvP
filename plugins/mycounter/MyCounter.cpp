
#include "MyCounter.h"
#include <QDebug>

MyCounter::MyCounter(QObject *parent) : QObject(parent) {
    qDebug() << "[Plugin] MyCounter::constructor";
}

int MyCounter::value() const { return m_value; }

void MyCounter::setValue(int newValue) {
    if (m_value != newValue) {
        m_value = newValue;
        emit valueChanged();
        qDebug() << "[Plugin] MyCounter::setValue ->" << m_value;
    }
}
