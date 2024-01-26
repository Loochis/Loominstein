cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 14)

project(Loominstein)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
include_directories(.)

add_subdirectory(lib/asciichis)
add_subdirectory(lib/mathchis)
add_subdirectory(lib/texturechis)

file(GLOB SRC_FILES *.cpp)
add_executable(Loominstein ${SRC_FILES})

target_link_libraries(Loominstein PRIVATE ${CURSES_LIBRARIES} asciichis mathchis texturechis)

set_target_properties(Loominstein PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ../
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ../
)
