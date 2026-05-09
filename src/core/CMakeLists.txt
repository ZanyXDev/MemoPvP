set(MODULE_NAME core) 
set(LIB_NAME ${MODULE_NAME}Lib)

qt_add_library(${LIB_NAME} STATIC)

set_target_properties(${LIB_NAME} PROPERTIES AUTOMOC ON)
target_link_libraries(${LIB_NAME} PRIVATE Qt6::Quick)

list(APPEND MODULE_SRC_FILES
    appconfig.h
    appconfig.cpp
    boardmodel.h
    boardmodel.cpp
    cellitem.h
    boardgenerator.h
    boardgenerator.cpp
    gameengine.h 
    gameengine.cpp
    boardmodel.h 
    boardmodel.cpp
    cellitem.h
  #  levelsinfo.h
  #  levelsinfo.cpp
)

qt_add_qml_module(${LIB_NAME}
    URI src.core
    VERSION 1.0
    RESOURCE_PREFIX /
    SOURCES
    ${MODULE_SRC_FILES}
)
