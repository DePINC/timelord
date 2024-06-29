cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0077 NEW)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

project(timelord)

option(BUILD_TEST "Build and run tests" ON)

find_package(Threads REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(plog CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(Boost 1.82.0 REQUIRED COMPONENTS system url)
find_package(OpenSSL REQUIRED)

set(BUILD_TEST OFF)

include(FetchContent)
FetchContent_Declare(
    vdf
    GIT_REPOSITORY https://github.com/depinc/vdf
    GIT_TAG v1.0.3
)

FetchContent_Declare(
    univalue
    GIT_REPOSITORY https://github.com/jgarzik/univalue
    GIT_TAG v1.0.5
)

FetchContent_Declare(
    tinyformat
    GIT_REPOSITORY https://github.com/c42f/tinyformat
    GIT_TAG v2.3.0
)

FetchContent_MakeAvailable(vdf univalue tinyformat)

set(CMAKE_CXX_STANDARD 17)

file(GLOB TIMELORDLIB_SRCS ./src/*.cpp)
file(GLOB TIMELORD_SRCS ./src/timelord/main.cpp ${TIMELORDLIB_SRCS})

# UniValue
set(UniValue_Root ${univalue_SOURCE_DIR})
set(UniValue_Include ${UniValue_Root}/include)
set(UniValue_Lib ${UniValue_Root}/lib)
file(GLOB UniValue_Sources ${UniValue_Lib}/*.cpp)

# TinyFormat
set(tinyformat_Include ${tinyformat_SOURCE_DIR})

add_library(univalue STATIC ${UniValue_Sources})
target_include_directories(univalue PUBLIC ${UniValue_Include} ${UniValue_Lib})

add_executable(timelord ${TIMELORD_SRCS})
add_dependencies(timelord time_vdf)
target_include_directories(timelord PRIVATE ${vdf_SOURCE_DIR}/src ${tinyformat_Include} ./src)
target_link_libraries(timelord PRIVATE
    time_vdf
    cxxopts::cxxopts
    plog::plog
    JsonCpp::JsonCpp
    fmt::fmt
    CURL::libcurl
    unofficial::sqlite3::sqlite3
    Boost::url
    OpenSSL::SSL
    OpenSSL::Crypto
    univalue
)

if (BUILD_TEST)
    function(MakeTest TEST_TARGET_NAME)
        set(TEST_SRCS
            ${TIMELORDLIB_SRCS}
            ./src/test/test_utils.cpp
            ./src/test/test_main.cpp
            ./src/test/${TEST_TARGET_NAME}.cpp
        )
        add_executable(${TEST_TARGET_NAME} ${TEST_SRCS})
        add_dependencies(${TEST_TARGET_NAME} time_vdf)
        target_include_directories(${TEST_TARGET_NAME} PRIVATE ${vdf_SOURCE_DIR}/src ${tinyformat_Include} ./src)
        target_link_libraries(${TEST_TARGET_NAME} PRIVATE
            time_vdf
            GTest::gtest
            plog::plog
            JsonCpp::JsonCpp
            fmt::fmt
            CURL::libcurl
            unofficial::sqlite3::sqlite3
            Boost::url
            OpenSSL::SSL
            OpenSSL::Crypto
            univalue
        )
        add_test(NAME ${TEST_TARGET_NAME} COMMAND ./${TEST_TARGET_NAME})
    endfunction()

    find_package(GTest CONFIG REQUIRED)

    enable_testing()
    MakeTest(test_frontend)
    MakeTest(test_timelord)
    MakeTest(test_vdf_client)
    MakeTest(test_challenge_monitor)
    MakeTest(test_sqlite)
    MakeTest(test_vdf_persist)
    MakeTest(test_web_service)
    MakeTest(test_block_querier)
    MakeTest(test_ip_addr_querier)
    MakeTest(test_gears)
endif()