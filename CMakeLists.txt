cmake_minimum_required(VERSION 3.7)
project(Matrix)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(Matrix ${SOURCE_FILES})

set(INCLUDE_DIR include)
set(SRC_DIR src)
set(HEADERS ${INCLUDE_DIR}/matrix.h)
set(SRCS ${SRC_DIR}/matrix.cpp)

include_directories(${INCLUDE_DIR})
add_library(matrix ${HEADERS} ${SRCS})