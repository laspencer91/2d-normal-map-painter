cmake_minimum_required(VERSION 3.30)
project(2d_normal_map_painter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "/Users/loganspencer/Qt/6.8.2/macos/lib/cmake")

find_package(Qt6 COMPONENTS Core Gui Widgets Widgets OpenGL OpenGLWidgets REQUIRED)

add_executable(2d_normal_map_painter main.cpp
        mainwindow.cpp
        mainwindow.h
        normalcanvas.cpp
        normalcanvas.h
        normalsphere.cpp
        normalsphere.h)
target_link_libraries(2d_normal_map_painter
  Qt::Core
  Qt::Gui
  Qt::Widgets
  Qt6::OpenGLWidgets
)

include_directories(${CMAKE_SOURCE_DIR}/glm)

