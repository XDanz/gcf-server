cmake_minimum_required(VERSION 3.2)
project(intro)

project(gcf_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
include_directories(include)
set(SOURCE_FILES src/FakeGCFServer.c src/DieWithMessage.c include/Pratical.h src/Rio.c include/Rio.h include/SoupBin.h src/GcfDataReader.c include/GcfDataReader.h)

add_executable(gcf_server ${SOURCE_FILES})

add_executable(readFromFile src/readFromFile.c)
add_executable(test_snprintf src/test_snprintf.c)

add_executable(mc_server src/FakeMultiCastProducer.c)

add_executable(mc_client src/MultiCastListener.c)
add_executable(Udp_client src/UdpClient.c)
add_executable(udp_server src/UdpServer.c)
add_executable(mc_proxy src/MultiCastProxy.c)

add_executable(test_fill src/test.c)
add_executable(gcf_server2 src/FakeGCFServer2.c src/DieWithMessage.c src/Practical.c include/Pratical.h src/writen.c src/Rio.c include/Rio.h src/SoupBin.c)
add_executable(gcf_test src/GcfDataReader.c include/GcfDataReader.h test/GCFDataReaderTest.c)
