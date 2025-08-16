cmake_minimum_required(VERSION 3.10)
project(FizBuzz C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
add_executable(fizbuzz src/main.c)
