#! /bin/sh

# 1. Очистка (если была ошибка конфигурации)
rm -rf build/*
mkdir -p build && cd build

# 2. Конфигурация с явным Debug и путём к Qt (если нужно)
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6 \
      ..

# 3. Сборка
cmake --build . -j$(nproc)

# 4. Запуск из Qt Creator:
#    - Откройте проект через корневой CMakeLists.txt
#    - Выберите конфигурацию "Debug"
#    - В "Projects → Run" убедитесь, что исполняемый файл — это app/memopvp
#    - Поставьте брейкпоинт в MyCounter.cpp → F5
