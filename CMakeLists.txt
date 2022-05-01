cmake_minimum_required(VERSION 3.22)
project(qt_projectile_test)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64/lib/cmake")
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)

add_executable(qt_projectile_test main.cpp mainwindow.cpp mainwindow.h view.cpp view.h collision_stuff/vector.h items/ball.cpp items/ball.h items/line.cpp items/line.h items/wall.cpp items/wall.h collision_stuff/vector.cpp collision_stuff/box.cpp collision_stuff/box.h global_config.h)

target_link_libraries(qt_projectile_test Qt5::Core)
target_link_libraries(qt_projectile_test Qt5::Widgets)
target_link_libraries(qt_projectile_test Qt5::Quick)