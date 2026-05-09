#! /bin/bash
set -x
mkdir -p /home/developer/src/MemoPvP/build/Desktop_amd64-Debug
cd /home/developer/src/MemoPvP/build/Desktop_amd64-Debug

# 2. Переконфигурация
cmake ../.. -DCMAKE_BUILD_TYPE=Debug

# 3. Сборка
cmake --build . --target memopvp

# 4. Проверка, что плагин установлен в правильную директорию
find . -path "*/qt/qml/ImageProviderCore/*" -type f
# Ожидаемый вывод:
# ./qt/qml/ImageProviderCore/qmldir
# ./qt/qml/ImageProviderCore/libqmlImageProviderPlugin.so
# ./qt/qml/ImageProviderCore/ImageProviderCore.qmltypes (опционально)

# 5. Запуск (указываем QML import path для разработки)
export QML2_IMPORT_PATH="$PWD/qt/qml:$QML2_IMPORT_PATH"
./memopvp
