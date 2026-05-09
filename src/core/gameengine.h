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
    Q_PROPERTY(QStringList imagePacks
                   READ imagePacks
                   NOTIFY imagePacksChanged FINAL)
public:
    explicit GameEngine(QObject* parent = nullptr);
    ~GameEngine() override = default;

    // Геттер для Q_PROPERTY
    BoardModel* boardModel() const { return m_boardModel; }

    // Q_INVOKABLE void startNewGame(bool smallBoard);
    // Q_INVOKABLE void selectImagePack(int pack_id);

    QStringList imagePacks() const { return m_imagePacks; }

signals:
    void imagePacksChanged();

private:
    BoardModel* m_boardModel = nullptr;
    BoardGenerator* m_boardGenerator = nullptr;

    QStringList m_imagePacks;
};
