#pragma once
#include <QObject>
#include <QtQml>

// Forward declaration: не тянем полные заголовки, ускоряем компиляцию
class BoardModel;
class BoardGenerator;

class GameEngine : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON  // ← Ключевая метка для Qt6
    // CONSTANT означает: указатель на модель создаётся раз и навсегда.
    // QML не будет переподписываться на новый сигнал изменения.
    Q_PROPERTY(BoardModel* boardModel READ boardModel CONSTANT)


    Q_PROPERTY(bool gameModeEasy READ gameModeEasy WRITE setGameModeEasy NOTIFY gameModeEasyChanged FINAL)
    Q_PROPERTY(int cellsInBoard READ cellsInBoard WRITE setCellsInBoard NOTIFY cellsInBoardChanged FINAL)


public:
    explicit GameEngine(QObject* parent = nullptr);
    ~GameEngine() override = default;

    // Геттер для Q_PROPERTY
    BoardModel* boardModel() const { return m_boardModel; }
    bool gameModeEasy() const;
    void setGameModeEasy(bool newGameMode);

    int cellsInBoard() const;
    void setCellsInBoard(int newCellsInBoard);
signals:
    void imagePacksChanged();

    void gameModeEasyChanged();

    void cellsInBoardChanged();

private:
    BoardModel* m_boardModel = nullptr;
    BoardGenerator* m_boardGenerator = nullptr;
    bool m_gameModeEasy;
    int m_cellsInBoard;

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
    void init();
};
