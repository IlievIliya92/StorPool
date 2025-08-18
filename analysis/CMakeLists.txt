cmake_minimum_required(VERSION 3.10)
project(Analysis C)

set(ARCHIVE_URL "https://quiz.storpool.com/bigf.json.bz2")
set(INPUT_DATA "${CMAKE_BINARY_DIR}/bigf.json.bz2")
set(INPUT_DATA_DEST_DIR "${CMAKE_BINARY_DIR}/")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

include(FetchContent)
FetchContent_Declare(
  yajl
  GIT_REPOSITORY https://github.com/lloyd/yajl.git
  GIT_TAG master
)
FetchContent_MakeAvailable(yajl)

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(analysis ${SOURCES})
target_include_directories(analysis PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(analysis PRIVATE ${CMAKE_BINARY_DIR}/_deps/yajl-build/yajl-2.1.1/include/)
target_link_libraries(analysis PRIVATE yajl)
