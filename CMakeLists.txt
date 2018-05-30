cmake_minimum_required(VERSION 3.5.1)
project(TIN)

#set(CMAKE_CXX_COMPILER /usr/bin/g++-7)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Boost 1.58.0 COMPONENTS serialization system filesystem REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(./inc)

set(SOURCE_FILES src/system/CommunicationModule.cc src/sockets/FDSet.cc src/sockets/TCPSocketBase.cc src/sockets/TCPServer.cc src/sockets/SocketHelpers.cc src/sockets/SocketSupervisor.cc src/sockets/SocketUnix.cc src/sockets/TCPSocket.cc src/messages/Message.cc src/messages/Acknowledge.cc src/queue/FileStorage.cc src/queue/Exceptions.cc  src/system/Exceptions.cc)
set(SOURCE_FILES_TESTS tests/tests_main.cc tests/messages/test_Acknowledge.cc tests/queue/testFileStorage.cc tests/system/testCommunicationModule.cc tests/system/TestMessage.h)

add_executable(TIN_TESTS ${SOURCE_FILES} ${SOURCE_FILES_TESTS})


target_link_libraries(TIN_TESTS ${GTEST_LIBRARIES})
add_executable(TIN ${SOURCE_FILES} src/main.cc)
add_executable(TIN_COM ${SOURCE_FILES} src/communicator.cc)
add_executable(TIN_A ${SOURCE_FILES} tests/testPrograms/A.cc)
add_executable(TIN_B ${SOURCE_FILES} tests/testPrograms/B.cc)
add_executable(TIN_C ${SOURCE_FILES} tests/testPrograms/C.cc)

if(Boost_FOUND)
    target_link_libraries(TIN_TESTS ${Boost_LIBRARIES})
    target_link_libraries(TIN ${Boost_LIBRARIES})
    target_link_libraries(TIN_A ${Boost_LIBRARIES})
    target_link_libraries(TIN_B ${Boost_LIBRARIES})
    target_link_libraries(TIN_C ${Boost_LIBRARIES})
endif()
