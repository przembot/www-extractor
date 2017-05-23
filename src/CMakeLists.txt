cmake_minimum_required(VERSION 3.5.1)
project(extractor)
set(CMAKE_CXX_STANDARD 14)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

set(SOURCE_FILES_QUERY
    querylexer.cpp
    queryparser.cpp
    )

set(SOURCE_FILES_HTML
    htmllexer.cpp
    htmlparser.cpp
    htmlvisitor.cpp
    )

set(SOURCE_FILES_OTHER
    extractor.cpp
    source.cpp
    )

set(SOURCE_FILES_MAIN
    main.cpp
    )

include_directories(${CMAKE_SOURCE_DIR}/.)
include_directories(${CMAKE_SOURCE_DIR}/test)

set(SOURCE_FILES_EXTRACTOR_TESTS
    test/test.cpp
    )

add_library(extractor_lib
    ${SOURCE_FILES_QUERY}
    ${SOURCE_FILES_HTML}
    ${SOURCE_FILES_OTHER}
    )

add_executable(extractor
    ${SOURCE_FILES_MAIN})

target_link_libraries(extractor
    extractor_lib)

add_executable(extractor_tests
    ${SOURCE_FILES_EXTRACTOR_TESTS})


target_link_libraries(extractor_tests
    extractor_lib
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

add_test(extractor_tests extractor_tests)
