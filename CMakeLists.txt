cmake_minimum_required(VERSION 3.10)
project(TIN)

set(CMAKE_CXX_STANDARD 17)
find_package (GTest REQUIRED Boost COMPONENTS serialization)
include_directories(${GTEST_INCLUDE_DIRS})
#include_directories( ${Boost_INCLUDE_DIRS} )

add_executable(runTests tests/tests_main.cc src/packages/Package.cc src/packages/Acknowledge.cc inc/packages/Acknowledge.h inc/packages/Package.h tests/packages/AcknowledgeTest.cc)
#target_link_libraries(tin ${Boost_SERALIZATION})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread -lboost_serialization)