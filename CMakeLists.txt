cmake_minimum_required(VERSION 3.16)

project(market_data_feed_simulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

enable_testing()
find_package(GTest)

file(GLOB_RECURSE LIB_SOURCES "src/*.cc")
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*/main\\.cc$")
file(GLOB_RECURSE TEST_SOURCES "test/*.cc")

add_library(market_data_feed_simulator_lib ${LIB_SOURCES})
add_executable(market_data_feed_simulator src/main.cc)

target_link_libraries(market_data_feed_simulator PRIVATE market_data_feed_simulator_lib)
if(GTest_FOUND)
    add_executable(market_data_feed_simulator_test ${TEST_SOURCES})
    target_link_libraries(market_data_feed_simulator_test 
        PRIVATE
        market_data_feed_simulator_lib
        GTest::gtest 
        GTest::gtest_main
    )
    include(GoogleTest)
    gtest_discover_tests(market_data_feed_simulator_test)
endif()
