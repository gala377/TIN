cmake_minimum_required(VERSION 3.6)
project(TIN)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Boost 1.58.0 COMPONENTS serialization system filesystem REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(inc/)

set(SOURCE_FILES src/sockets/TCPSocket.cc src/sockets/TCPServer.cc src/sockets/TCPSocketBase.cc src/sockets/SocketSupervisor.cc src/sockets/SocketUnix.cc src/sockets/TCPSocketWithPacket.cc src/messages/Message.cc src/messages/Acknowledge.cc)
set(SOURCE_FILES_TESTS tests/tests_main.cc tests/messages/test_Acknowledge.cc)

if(FALSE)
    add_executable(TIN_TESTS ${SOURCE_FILES} ${SOURCE_FILES_TESTS})
    if(Boost_FOUND)
        target_link_libraries(TIN_TESTS ${Boost_LIBRARIES})
    endif()

    target_link_libraries(TIN_TESTS ${GTEST_LIBRARIES})
else()
    add_executable(TIN ${SOURCE_FILES} ${SOURCE_FILES} src/main.cpp)
    if(Boost_FOUND)
        target_link_libraries(TIN ${Boost_LIBRARIES})
    endif()
endif()