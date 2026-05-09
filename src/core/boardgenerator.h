#pragma once
#include <QObject>

class BoardGenerator : public QObject
{
    Q_OBJECT

public:
    explicit BoardGenerator(QObject *parent = nullptr);
    ~BoardGenerator() override = default;
    /**
     * @brief generateBoard(bool smallBoard,quint32 seed = 0)
     * @value smallBoard [true/false] Мальнькая 12 ячеек, больша 16 ячеек доска
     * @value seed начальная точка для генератора ПСП
     * @return QVector<int>
     * @note Генерация игровой доски в виде вектора
     * @note Атрибут [[nodiscard]] (появился в C++17) — инструмент проверки кода,
    *       заставляющий компилятор выдавать предупреждение, если результат функции,
    *       помеченной этим атрибутом, игнорируется.
    *       Он используется для функций, где неиспользование возвращаемого значения
    *       (например, кода ошибки или ресурса) почти наверняка является ошибкой
    *       программиста.
    */
    [[nodiscard]] QVector<int> generateBoard(bool smallBoard,quint32 seed = 0);

signals:
    void boardCreated();

private:    
    // ✅ compile-time константа, общая для всех объектов
    static constexpr int kLevelsSize = 48;  // k-префикс для констант
};

