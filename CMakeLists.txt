cmake_minimum_required(VERSION 3.30)
project(2d_normal_map_painter)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.8.2/mingw_64")

find_package(Qt6 COMPONENTS Core Gui Widgets Widgets OpenGL OpenGLWidgets REQUIRED)

add_executable(2d_normal_map_painter WIN32
        main.cpp
        MainWindow.cpp
        MainWindow.h
        NormalCanvas.cpp
        NormalCanvas.h
        NormalSelectorSphere.cpp
        NormalSelectorSphere.h
        PreviewCanvas.cpp
        PreviewCanvas.h)

target_link_libraries(2d_normal_map_painter
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGLWidgets
)

include_directories(${CMAKE_SOURCE_DIR}/glm)