cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
project(u)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")

if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "-std=c99 ${CMAKE_C_FLAGS}")
    endif()
else()
    set(CMAKE_C_STANDARD 99)
endif()

set(u_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(u_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(u_TST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

file(GLOB u_HEADERS ${u_HEADERS} ${u_INC_DIR}/*.h)
file(GLOB u_SOURCES ${u_SOURCES} ${u_SRC_DIR}/*.c)

include_directories(${u_INC_DIR})
add_library(${PROJECT_NAME} STATIC ${u_SOURCES} ${u_HEADERS})
target_link_libraries(${PROJECT_NAME} pthread m dl)

enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

unset(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
include_directories(${u_TST_DIR})

set(CMAKE_C_FLAGS -Wno-int-conversion)

file(GLOB adt_TST_DIRS ${adt_TST_DIRS} ${u_TST_DIR}/*.c)
foreach(test ${adt_TST_DIRS})
    get_filename_component(test_NAME ${test} NAME_WE)
    if(NOT ${test_NAME} STREQUAL test)
        add_executable(${test_NAME} ${test})
        add_test(test_${test_NAME} ${test_NAME})
        target_link_libraries(${test_NAME} ${PROJECT_NAME})
        add_dependencies(check ${test_NAME})
    endif()
endforeach()
