cmake_minimum_required(VERSION 3.27)
project(Loominstein)

set(CMAKE_CXX_STANDARD 14)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
include_directories(.)

add_library(AsciiChis asciichis.cpp asciichis.h)
add_library(MathChis  mathchis.cpp  mathchis.h)

add_executable(Loominstein
        asciichis.cpp
        asciichis.h
        main.cpp)

target_link_libraries(Loominstein PRIVATE ${CURSES_LIBRARIES} AsciiChis MathChis)
