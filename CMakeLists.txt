cmake_minimum_required(VERSION 3.5)
CMAKE_POLICY(SET CMP0020 NEW)

project(MiraclePlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(Qt6 COMPONENTS Multimedia REQUIRED) # 请求多媒体包

# 本项目需要的所有文件
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        albumitem.cpp
        albumitem.h
        albumitem.ui
        albumview.cpp
        albumview.h
        albumview.ui
        playercontrol.cpp
        playercontrol.h
        playercontrol.ui
        sidebaritem.cpp
        sidebaritem.h
        sidebaritem.ui
        musiclist.cpp
        musiclist.h
        musiclist.ui
        musicitem.cpp
        musicitem.h
        musicitem.ui
        musicdata.h
        favview.cpp
        favview.h
        favview.ui
        searchform.cpp
        searchform.h
        searchform.ui
        settings.cpp
        settings.h
        settings.ui
        playsoundhelper.h
        playsoundhelper.cpp
        playerhelper.h
        metadatahelper.h
        player-assets.qrc
        app_win32.rc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MiraclePlayer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MiraclePlayer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MiraclePlayer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MiraclePlayer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(MiraclePlayer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(MiraclePlayer PUBLIC Qt::Multimedia) # 引入多媒体支持

set_target_properties(MiraclePlayer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER www.miracleplayer.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# 去掉控制台窗口，改用mainCRTStartup入口点
if(WIN32)
    set_target_properties(MiraclePlayer PROPERTIES
        WIN32_EXECUTABLE YES
        LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MiraclePlayer)
endif()
