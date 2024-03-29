cmake_minimum_required(VERSION 3.22)
project(blockade_runner)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_library(blockadeRunnerLib)
add_subdirectory(main_window)
add_subdirectory(views)
add_subdirectory(physics)
add_subdirectory(effects)
add_subdirectory(menu_items)
add_subdirectory(tactical_items)
add_subdirectory(strategic_items)
add_subdirectory(terminal)
add_subdirectory(models)
add_subdirectory(world_objects)

target_link_libraries(blockadeRunnerLib PUBLIC Qt5::Widgets)

add_executable(blockade_runner main.cpp)

target_link_libraries(blockade_runner PUBLIC Qt5::Core Qt5::Widgets blockadeRunnerLib)