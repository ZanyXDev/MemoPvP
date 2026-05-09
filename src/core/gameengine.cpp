#include "gameengine.h"
#include "boardmodel.h"


GameEngine::GameEngine(QObject* parent)
    : QObject(parent)
    // 👇 ВАЖНО: передаём `this` как parent. Qt-дерево объектов автоматически удалит модель.
    , m_boardModel(new BoardModel(this))
{
    m_imagePacks.append(tr("Фрукты и ягоды"));
    m_imagePacks.append(tr("Домашние животные"));
    m_imagePacks.append(tr("Дикие животные"));
}

