cmake_minimum_required(VERSION 3.5)
project(intro)

project(gcf_server)
find_package( Threads REQUIRED )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
include_directories(include)
set(SOURCE_FILES src/FakeGCFServer.c src/DieWithMessage.c include/Pratical.h src/Rio.c include/Rio.h)

add_executable(gcf_server ${SOURCE_FILES})
target_link_libraries(gcf_server "${CMAKE_THREAD_LIBS_INIT}")

add_executable(readFromFile src/readFromFile.cpp)
add_executable(test_snprintf src/test_snprintf.c)
add_executable(get_pid src/getpid.c)

add_executable(mc_server src/FakeMultiCastProducer.cpp)

add_executable(mc_client src/MultiCastListener.c)
add_executable(Udp_client src/UdpClient.c)
add_executable(udp_server src/UdpServer.c)
add_executable(mc_proxy src/MultiCastProxy.c)

add_executable(test_fill src/test.c)
add_executable(ts src/Test.c)
add_executable(strtoul src/strtoul.c)
add_executable(getopts src/getopts.c)
target_link_libraries(ts "${CMAKE_THREAD_LIBS_INIT}")
