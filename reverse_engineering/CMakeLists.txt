cmake_minimum_required(VERSION 3.18)
project(ReverseEngineering LANGUAGES C)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")

set(URL "https://quiz.storpool.com/binaries.tgz")
set(OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/storpool_binaries/")
set(SHA256 "666cfd62c6dcebe39b2d88bb11b86dadde45dc50b14f5aca155aacf636a92303")
set(OUT_FILE "${OUT_DIR}/binaries.tgz")

if(NOT EXISTS "${OUT_FILE}")
  message(STATUS "Downloading ${URL} -> ${OUT_FILE}")
  file(DOWNLOAD
    "${URL}"
    "${OUT_FILE}"
    SHOW_PROGRESS
  )
else()
  message(STATUS "Already present: ${OUT_FILE}; skipping download")
endif()

file(ARCHIVE_EXTRACT
     INPUT "${OUT_FILE}"
     DESTINATION "${OUT_DIR}")

add_executable(a src/a.c)
add_executable(b src/b.c)

find_program(LLVM_STRIP llvm-strip)
if(LLVM_STRIP)
    add_custom_command(TARGET a POST_BUILD
        COMMAND ${LLVM_STRIP} $<TARGET_FILE:a>
        COMMENT "Stripping binary with llvm-strip"
    )
    add_custom_command(TARGET b POST_BUILD
        COMMAND ${LLVM_STRIP} $<TARGET_FILE:b>
        COMMENT "Stripping binary with llvm-strip"
    )
else()
    message(WARNING "llvm-strip not found, binaries will not be stripped automatically")
endif()
