cmake_minimum_required(VERSION 3.19)
project(utfx C)

set(CMAKE_C_STANDARD 90)

add_library(utfx utfx.c utfx.h)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
    enable_testing()

    # does the application run
    add_test(NAME RunAllTests COMMAND utfx_tests)
    set_tests_properties(RunAllTests
            PROPERTIES WILL_FAIL FALSE
            )
endif()


