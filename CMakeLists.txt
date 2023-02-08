cmake_minimum_required(VERSION 3.24)
project(WorldBuildr)

set(UIS src/GameFramework/mainfrm.ui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH C:/Qt/6.4.0/mingw_64/)
set(CMAKE_AUTOMOC TRUE)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

include_directories(src/GameFramework src/WorldBuildrEditor src/WorldBuildrUi src)

add_executable(WorldBuildr
        src/GameFramework/main.cpp
        src/GameFramework/mainfrm.ui
        src/GameFramework/mainfrm.cpp src/GameFramework/mainfrm.h
        src/GameFramework/gameview.cpp src/GameFramework/gameview.h
        src/GameFramework/gamecanvas.cpp src/GameFramework/gamecanvas.h
        src/GameFramework/gamescene.cpp src/GameFramework/gamescene.h
        src/GameFramework/spritetickhandler.cpp src/GameFramework/spritetickhandler.h
        src/GameFramework/resources.cpp src/GameFramework/resources.h
        src/GameFramework/sprite.cpp src/GameFramework/sprite.h
        src/GameFramework/utilities.cpp src/GameFramework/utilities.h
        src/WorldBuildrEditor/gamecore.cpp src/WorldBuildrEditor/gamecore.h
        src/WorldBuildr.pro
        src/WorldBuildrEditor/EditorSprite.cpp src/WorldBuildrEditor/EditorSprite.h
        src/WorldBuildrEditor/SelectionZone.cpp src/WorldBuildrEditor/SelectionZone.h
        src/WorldBuildrEditor/EditorManager.cpp src/WorldBuildrEditor/EditorManager.h
        src/WorldBuildrUi/EditorActionPanel.cpp src/WorldBuildrUi/EditorActionPanel.h
        src/WorldBuildrEditor/EditorHistory.cpp src/WorldBuildrEditor/EditorHistory.h src/WorldBuildrEditor/EditorHistory.h src/WorldBuildrEditor/SaveFileManager.cpp src/WorldBuildrEditor/SaveFileManager.h src/WorldBuildrEditor/SaveFileManager.h)
target_link_libraries(WorldBuildr
        Qt::Core
        Qt::Gui
        Qt6::Widgets
        )

if (WIN32)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
