#Чтобы qmllint и QtCreator находили модуль при разработке,
set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qt/qml")

# Qt автоматически сгенерирует QQmlExtensionPlugin и qmldir
qt_add_qml_module(MyQmlPlugin
    URI io.github.zanyxdev.memopvp.myplugin
    VERSION 1.0
    SOURCES
        MyCounter.cpp MyCounter.h
)
