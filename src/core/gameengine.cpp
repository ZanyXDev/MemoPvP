#include "gameengine.h"
#include "boardmodel.h"
#include "boardgenerator.h"


GameEngine::GameEngine(QObject* parent)
    : QObject(parent)
    // 👇 ВАЖНО: передаём `this` как parent. Qt-дерево объектов автоматически удалит модель.
    , m_boardModel(new BoardModel(this))
    , m_boardGenerator (new BoardGenerator(this))
    , m_gameModeEasy (true)
    , m_cellsInBoard(12)
{
#ifdef QT_DEBUG
        QStringList paths;
        scanDirectoryPaths("://qt/qml/shadereffects/assets/shaders", paths);
        //qDebug() << "[DEV] All resources:" << paths;
#endif
}

void GameEngine::startNewGame()
{

}


bool GameEngine::gameModeEasy() const
{
    return m_gameModeEasy;
}

void GameEngine::setGameModeEasy(bool newGameMode)
{
    if (m_gameModeEasy == newGameMode)
        return;
    m_gameModeEasy = newGameMode;
    (m_gameModeEasy) ? setCellsInBoard(12): setCellsInBoard(16);
    emit gameModeEasyChanged();
}

int GameEngine::cellsInBoard() const
{
    return m_cellsInBoard;
}

void GameEngine::setCellsInBoard(int newCellsInBoard)
{
    if (m_cellsInBoard == newCellsInBoard)
        return;

    m_cellsInBoard = newCellsInBoard;
    emit cellsInBoardChanged();
}

void GameEngine::init()
{

}


int GameEngine::imagesInPack() const
{
    return m_imagesInPack;
}

void GameEngine::setImagesInPack(int newImagesInPack)
{
    if (m_imagesInPack == newImagesInPack)
        return;
    m_imagesInPack = newImagesInPack;
    emit imagesInPackChanged();
}
