cmake_minimum_required(VERSION 3.6)
project(ConCurrency)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp simple_time_stamp.h)
add_executable(ConCurrency ${SOURCE_FILES})