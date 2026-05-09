#include "levelsinfo.h"

LevelsInfo::LevelsInfo(QObject* parent)
    : QObject(parent)
{
}


QString &LevelsInfo::levelName() const
{
    return m_levelName;
}
