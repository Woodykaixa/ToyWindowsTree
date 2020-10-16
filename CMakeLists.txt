cmake_minimum_required(VERSION 3.16)
project(WindowsTree)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-D UNICODE)

add_subdirectory(lib/fmt EXCLUDE_FROM_ALL)
add_executable(WindowsTree main.cpp FileScanner.cpp FileScanner.h ArgParser.cpp ArgParser.h StupidEncodingUtil.h)
target_link_libraries(WindowsTree fmt)
