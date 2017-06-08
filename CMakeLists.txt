cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(u)

if ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    set(CLANG TRUE)
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
    set(GCC TRUE)
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "Intel")
    set(ICC TRUE)
elseif(NOT MSVC)
    message(FATAL_ERROR "Unknown compiler")
endif()

option(COVERAGE "Turn on COVERAGE support" OFF)

if(CMAKE_VERSION VERSION_LESS "3.1")
    if(GCC)
        set(CMAKE_C_FLAGS "-std=c99 ${CMAKE_C_FLAGS}")
    endif()
else()
    set(CMAKE_C_STANDARD 99)
endif()

if(COVERAGE AND NOT MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
endif()

set(u_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(u_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(u_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)
set(u_VENDOR_DIR ${CMAKE_CURRENT_LIST_DIR}/vendor)

file(GLOB_RECURSE u_HEADERS ${u_HEADERS} ${u_INCLUDE_DIR}/${PROJECT_NAME}/*.h)
file(GLOB_RECURSE u_SOURCES ${u_SOURCES} ${u_SOURCE_DIR}/*.c)

add_library(${PROJECT_NAME} STATIC ${u_SOURCES} ${u_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${u_HEADERS}")
target_include_directories(${PROJECT_NAME} PUBLIC ${u_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${u_SOURCE_DIR})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Oy)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE /Od)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE /Ox)
    endif()
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wextra -fomit-frame-pointer)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -O0 -g3)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -O3)
    endif()
endif()

if(MSVC)
    set(CMAKE_FLAGS
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
    )
    foreach(CMAKE_FLAG ${CMAKE_FLAGS})
        string(REPLACE "/MD" "/MT" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
        string(REPLACE "/MDd" "/MTd" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
    endforeach()
endif()

if(EXISTS ${u_TEST_DIR})
    enable_testing()
    add_subdirectory(${u_VENDOR_DIR}/cute)
    add_subdirectory(${u_TEST_DIR})
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)
install(FILES ${u_INCLUDE_DIR}/${PROJECT_NAME}.h
    DESTINATION include
)
