project(PCTUnittest)

add_executable(
    ${PROJECT_NAME}
    TestHostNameUtilities.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    PCT::LibPCT
    GTest::gtest_main
)
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

include(GoogleTest)

gtest_discover_tests(
    ${PROJECT_NAME}
    DISCOVERY_MODE PRE_TEST
)

