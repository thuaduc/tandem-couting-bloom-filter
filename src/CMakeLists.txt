cmake_minimum_required(VERSION 3.10)
project(T-CBF)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")


set(SOURCES ultility/murmurHash.cpp ultility/help.cpp filter/bf/bf.cpp filter/cbf/cbf.cpp filter/vbf/vbf.cpp filter/tbf/tbf.cpp)

set(LIBRARY_NAME filter)
set(EXECUTABLE_NAME main)


add_library(${LIBRARY_NAME} ${SOURCES})
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)

add_executable(${EXECUTABLE_NAME} main.cpp)
target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})

