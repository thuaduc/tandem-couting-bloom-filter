cmake_minimum_required(VERSION 3.10)
project(T-CBF)

find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")


add_executable(main main.cpp ultility/sha.cpp ultility/help.cpp filter/bf/bf.cpp ultility/murmurHash.cpp)

target_link_libraries(main PRIVATE OpenSSL::SSL OpenSSL::Crypto)
