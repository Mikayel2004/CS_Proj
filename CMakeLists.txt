cmake_minimum_required(VERSION 3.5)
project(GalaxySim LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGL)

# Добавляем OpenGLWidgets если это Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
endif()

add_executable(GalaxySim
  main.cpp
  quadtree.cpp
  quadtree.h
  simulationwidget.cpp
  simulationwidget.h
)

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(GalaxySim PRIVATE Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets)
else()
    target_link_libraries(GalaxySim PRIVATE Qt5::Widgets Qt5::OpenGL)
endif()
