cmake_minimum_required(VERSION 3.27)
project(Loominstein)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_library(AsciiChis asciichis.cpp asciichis.h)
add_library(MathChis  mathchis.cpp  mathchis.h)

add_executable(Loominstein
        asciichis.cpp
        asciichis.h
        main.cpp)

target_link_libraries(Loominstein PRIVATE AsciiChis MathChis)
