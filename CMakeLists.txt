cmake_minimum_required(VERSION 3.3)
project(intro)

project(gcf_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
include_directories(include)
set(SOURCE_FILES src/FakeGCFServer.c src/DieWithMessage.c include/Pratical.h)

add_executable(gcf_server ${SOURCE_FILES})

add_executable(readFromFile src/readFromFile.c)