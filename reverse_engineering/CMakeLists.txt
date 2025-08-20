cmake_minimum_required(VERSION 3.18)
project(ReverseEngineering LANGUAGES NONE)

set(URL "https://quiz.storpool.com/binaries.tgz")
set(OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/")
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
