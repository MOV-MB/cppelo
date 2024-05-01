cmake_minimum_required(VERSION 3.10)

project(EloSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/include/sqlite)

file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/include/*.h
        ${CMAKE_SOURCE_DIR}/src/include/sqlite/*.c)

add_executable(EloSystem ${SOURCE_FILES})

target_link_libraries(EloSystem pthread dl)
