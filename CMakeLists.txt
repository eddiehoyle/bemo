cmake_minimum_required(VERSION 3.5)

project(bemo)

#include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)


set(TARGET_DIR ${CMAKE_SOURCE_DIR}/build)
set(PYTHON_DIST_DIR ${TARGET_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TARGET_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TARGET_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TARGET_DIR})

set(PYBIND11 ${CMAKE_SOURCE_DIR}/ext/pybind11)
set(PYTHON_INCLUDE_DIR /usr/include/python2.7)
set(PYBIND11_PYTHON_VERSION 2.7 CACHE STRING "")

set(BMO_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cpp)
set(BMO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cpp)
set(BMO_EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext)

include_directories(${BMO_INCLUDE_DIR})

add_subdirectory(ext)
add_subdirectory(cpp)
add_subdirectory(test)