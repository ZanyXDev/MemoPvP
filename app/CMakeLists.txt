#Чтобы qmllint и QtCreator находили модуль при разработке,
set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qt/qml")

qt_add_executable(${APP_NAME} main.cpp)

qt_add_resources(${APP_NAME} "images"
    PREFIX
        "/qt/qml"
    FILES
    ../assets/images/__animals/animals_0.png
    ../assets/images/__animals/animals_1.png
    ../assets/images/__emoji/emoji_0.png
    ../assets/images/__emoji/emoji_1.png
    ../assets/images/__emoji/emoji_2.png
    ../assets/images/cardbackside.jpg
    ../assets/images/memory_logo.png
)

qt_add_resources(${APP_NAME} "fonts"
    PREFIX
        "/qt/qml"
    FILES
    ../assets/fonts/Buira/Buira.otf
)

qt6_add_shaders(${APP_NAME} "shaders"
    PRECOMPILE
    BATCHABLE
    # OPTIMIZED
    PREFIX
        "/qt/qml"
    FILES
        ../assets/shaders/flip.frag
        ../assets/shaders/flip.vert
)

set(QML_FILES
    qml/Main.qml
)

qt_add_qml_module(${APP_NAME}
    URI io.github.zanyxdev.memopvp
    VERSION 1.0
    QML_FILES ${QML_FILES}   
)

# После qt_add_qml_module(...) добавьте:
target_compile_definitions(${APP_NAME} PRIVATE
    $<$<CONFIG:Debug>:
        QML_DEV_IMPORT_PATH="${QT_QML_OUTPUT_DIRECTORY}"
    >
)

target_link_libraries(${APP_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick          # <-- обязательно, если есть #include <QQuickWindow>
)

install(TARGETS ${APP_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
