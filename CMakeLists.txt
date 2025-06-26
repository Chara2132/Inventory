cmake_minimum_required(VERSION 3.16)
project(InventoryApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)  # Questo abilita GNU extensions → gnu++23

# Per disabilitarle e usare il puro C++23: set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main
    src/main.cpp
    src/item_manager.cpp
)

# Trova GTKMM
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

target_link_libraries(main ${GTKMM_LIBRARIES})
