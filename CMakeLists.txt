cmake_minimum_required(VERSION 3.16)

# Читаем имя приложения из файла ДО project()
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/activity_name.txt" APP_NAME_RAW)
string(STRIP "${APP_NAME_RAW}" APP_NAME)

project(${APP_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# ------------------------- tune speed buld ----------------------------------
# 1. Включить Unity Build (объединяет .cpp файлы → меньше компиляций)
set(CMAKE_UNITY_BUILD OFF)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 4)  # баланс между скоростью и инкрементальностью

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 2. Отключить избыточную генерацию QML cache в Debug
    # (если не используете AOT-компиляцию QML в дебаге)
    set(QT_QMLCACHE_GENERATOR_AOT_COMPILER_ENABLED OFF)
endif()
# ----------------------------------------------------------------------------


################################################################################

message(STATUS "[CMAKE] CMAKE_VERSION               = ${CMAKE_VERSION}")
message(STATUS "[CMAKE] CMAKE_PREFIX_PATH           = ${CMAKE_PREFIX_PATH}")
message(STATUS "[SYSTEM] (HOST) SYSTEM NAME         = ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "[SYSTEM] (HOST) SYSTEM VERSION      = ${CMAKE_HOST_SYSTEM_VERSION}")
message(STATUS "[SYSTEM] (TARGET) SYSTEM NAME       = ${CMAKE_SYSTEM_NAME}")
message(STATUS "[SYSTEM] (TARGET) SYSTEM VERSION    = ${CMAKE_SYSTEM_VERSION}")
message(STATUS "[PROJECT] SOURCE_DIR                = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "[PROJECT] BUILD_TYPE                = ${CMAKE_BUILD_TYPE}")
message(STATUS "[PROJECT] TOOLCHAIN_FILE            = ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "[PROJECT] CMAKE_INSTALL_LIBDIR      = ${CMAKE_INSTALL_LIBDIR}")
message(STATUS "[PROJECT] QT_HOST_PATH              = ${QT_HOST_PATH}")


################################################################################
# ----------------------------------------------------------------------------
# Читаем метаданные проекта из файлов
# ----------------------------------------------------------------------------
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" APP_VERSION_RAW)
string(STRIP "${APP_VERSION_RAW}" APP_VERSION)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/package_name.txt" PACKAGE_NAME_RAW)
string(STRIP "${PACKAGE_NAME_RAW}" PACKAGE_NAME)

# Compile definitions (аналог DEFINES в .pro)
add_compile_definitions(
    VERSION_STR="${APP_VERSION}"
    PACKAGE_NAME_STR="${PACKAGE_NAME}"
    ACTIVITY_NAME_STR="${APP_NAME}"
    QT_DEPRECATED_WARNINGS
    # QT_NO_CAST_FROM_ASCII
    # QT_NO_CAST_TO_ASCII
)


# ----------------------------------------------------------------------------
# CCache интеграция (если установлен)
# ----------------------------------------------------------------------------
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")

    # Нормализация путей для отладочных сборок
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(
            -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=.
        )
    endif()

    # Для ccache: установить base_dir через окружение
    set(ENV{CCACHE_BASEDIR} "${CMAKE_SOURCE_DIR}")

    message(STATUS "✓ Using ccache: ${CCACHE_PROGRAM}")
    message(STATUS "✓ CCACHE_BASEDIR: $ENV{CCACHE_BASEDIR}")

endif()



find_package(Qt6 REQUIRED COMPONENTS Quick ShaderTools)

qt_standard_project_setup(REQUIRES 6.10)

#Чтобы qmllint и QtCreator находили модуль при разработке
# https://forum.qt.io/topic/146086/usage-of-import_path-in-qt_add_qml_module/4
set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qt/qml")
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY} CACHE STRING "Import paths for Qt Creator's code model" FORCE)


qt_add_executable(${APP_NAME}
    main.cpp
)

qt_add_resources(${APP_NAME} "images.packs"
    #PREFIX "/images/packs/animals"
    FILES
    assets/images/packs/animals/animals_0.png
    assets/images/packs/animals/animals_1.png
    assets/images/packs/emoji/emoji_0.png
    assets/images/packs/emoji/emoji_1.png
    assets/images/packs/emoji/emoji_2.png
    assets/images/cardbackside.jpg
)

set(QML_FILES
    qml/Main.qml
    qml/Pages/InitPage.qml    
    qml/Pages/TestPage.qml
    qml/Components/MemoCard.qml
    qml/Components/ShaderFlip.qml
    qml/Components/SimpleFlip.qml
)

set(RES_FILES
    assets/fonts/Buira/Buira.otf
    assets/images/memory_logo.png
)

qt_add_qml_module(${APP_NAME}
    URI io.github.zanyxdev.memopvp
    VERSION 1.0
    QML_FILES ${QML_FILES}
    RESOURCES ${RES_FILES}

)

qt6_add_shaders(${APP_NAME} "shaders"
    PRECOMPILE
    BATCHABLE
    # OPTIMIZED

    PREFIX
        "/qt/qml/shadereffects"
    FILES
        assets/shaders/flip.frag
        assets/shaders/flip.vert
)

add_subdirectory(src)

## Android
if(ANDROID)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        #QT_ANDROID_ABIS "armeabi-v7a;arm64-v8a;x86;x86_64"
        #QT_ANDROID_BUILD_ALL_ABIS ON
        # Minimal version Android 8.0
        QT_ANDROID_MIN_SDK_VERSION 26
        QT_ANDROID_TARGET_SDK_VERSION 36
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android/"
    )
    qt_add_android_permission(${CMAKE_PROJECT_NAME}
        NAME android.permission.BLUETOOTH_SCAN
        ATTRIBUTES
            minSdkVersion 31
            usesPermissionFlags neverForLocation
    )
    qt_add_android_permission(${CMAKE_PROJECT_NAME}
        NAME android.permission.ACCESS_COARSE_LOCATION
    )
endif()

# ----------------------------------------------------------------------------
# QTest интеграция
# ----------------------------------------------------------------------------
set(BUILD_TEST ON)
if (BUILD_TEST)
    enable_testing()
    # Добавляем подпроект с тестами
    add_subdirectory(tests)
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${APP_NAME} PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMemoPvP
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${APP_NAME}
    PRIVATE Qt6::Quick    
    coreLibplugin    
)

install(TARGETS ${APP_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
