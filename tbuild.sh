#! /bin/bash
set -x
cd /home/developer/src/MemoPvP/build/Desktop-Debug

# 2. Переконфигурация
cmake ../.. -DCMAKE_BUILD_TYPE=Debug

# Первая сборка (должны быть misses):
cmake --build Desktop-Debug
ccache -s

# Вторая сборка без изменений (должны быть hits ~90%:
cmake --build build/Desktop-Debug
ccache -s  # если Hits < 80% → проблема в настройках
