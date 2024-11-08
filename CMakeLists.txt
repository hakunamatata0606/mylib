cmake_minimum_required(VERSION 3.28)

project(
    MyLib
    VERSION 1.0
    LANGUAGES CXX
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thread_pool)