cmake_minimum_required(VERSION 3.6)
project(TIN)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Boost 1.58.0 COMPONENTS serialization system filesystem REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(./inc)

set(SOURCE_FILES src/sockets/TCPSocket.cc  src/sockets/TCPServer.cc  src/sockets/TCPSocketBase.cc
        src/sockets/SocketSupervisor.cc src/Example.cc  src/sockets/SocketUnix.cc src/sockets/TCPSocketWithPacket.cc
         src/messages/Message.cc  src/messages/Acknowledge.cc
        tests/messages/test_Acknowledge.cc src/qeueu/FileStorage.cc)
add_executable(TIN ${SOURCE_FILES})

if(Boost_FOUND)
    target_link_libraries(TIN ${Boost_LIBRARIES})
endif()

target_link_libraries(TIN ${GTEST_LIBRARIES})