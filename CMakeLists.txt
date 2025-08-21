cmake_minimum_required(VERSION 3.10)
project(StorPool C)

option(DOCS_BUILD "Build html docs" OFF)

add_subdirectory(analysis)
add_subdirectory(counting)
add_subdirectory(fizzbuzz)
add_subdirectory(reverse_engineering)

if(DOCS_BUILD)
    add_subdirectory(doc)
endif()
