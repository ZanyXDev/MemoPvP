#pragma once
#include <QObject>

class LevelsInfo : public QObject
{
    Q_OBJECT
    /**
    Q_PROPERTY(QString &levelName READ levelName FINAL)
    Q_PROPERTY(QString &descrition READ descrition FINAL)
    Q_PROPERTY(QString &uri READ uri FINAL)
    Q_PROPERTY(QString levelIconSource READ levelIconSource FINAL)
    Q_PROPERTY(int pictureCount READ pictureCount FINAL)
    Q_PROPERTY(int levelId READ levelId FINAL)
    */
public:
    explicit LevelsInfo(QObject* parent = nullptr);
    ~LevelsInfo() override = default;

    void selectLevel(int levelId);

private:
    QVector<int> m_levels;
    int m_currentLevel;
};
