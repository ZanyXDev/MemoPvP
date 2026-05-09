#include <QRandomGenerator>
#include <algorithm>
#include <random>  // для std::mt19937
#include <memory>

#include "boardgenerator.h"


BoardGenerator::BoardGenerator(QObject *parent)
{

}

QVector<int> BoardGenerator::generateBoard(bool smallBoard, quint32 seed)
{
    const int boardSize = smallBoard ? 12 : 16;
    const int halfSize = smallBoard ? 6 : 8;

    Q_ASSERT_X(kLevelsSize >= halfSize, "BoardGenerator::generateBoard",
               "kLevelsSize must be >= half of board size");



    // 1. Пул доступных индексов
    QVector<int> availableIndices(kLevelsSize);
    std::iota(availableIndices.begin(), availableIndices.end(), 0); // Вместо цикла

    // 2. Генератор для выбора уникальных индексов (Qt)

    std::unique_ptr<QRandomGenerator> rng;
    if (seed != 0) {
        rng = std::make_unique<QRandomGenerator>(seed);
    }
    auto* qtRng = rng ? rng.get() : QRandomGenerator::global();

    // Выбор halfSize уникальных индексов (алгоритм partial Fisher-Yates)
    // КЛЮЧЕВОЙ МОМЕНТ: bounded(i, max) возвращает [i, max), поэтому randomIdx >= i.
    // Это гарантирует, что мы меняем текущий элемент только с ещё не выбранными,
    // и никогда не затрагиваем уже зафиксированные значения в диапазоне [0, i-1].
    for (int i = 0; i < halfSize; ++i) {
        const int randomIdx = qtRng->bounded(i, kLevelsSize);
        std::swap(availableIndices[i], availableIndices[randomIdx]);
    }
    // После цикла первые halfSize элементов availableIndices содержат уникальные значения.

    // 3. Формируем пары
    // Локальный результат — никакой связи с состоянием объекта
    QVector<int> m_data;
    m_data.reserve(boardSize);

    for (int i = 0; i < halfSize; ++i) {
        const int pictureIndex = availableIndices[i];
        m_data.append(pictureIndex);
        m_data.append(pictureIndex);
    }

    // 4. Перемешивание через std::mt19937 (совместим со std::shuffle)
    std::mt19937 shuffleRng;
    if (seed != 0) {
        shuffleRng.seed(seed);  // Детерминированный режим        
    }
    // Если seed == 0: используем дефолтный seed mt19937
    std::shuffle(m_data.begin(), m_data.end(), shuffleRng);

    emit boardCreated();
    // Возврат локальной переменной (RVO/Move)
    return m_data;
}

