cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
project(u)

if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "-std=c99 ${CMAKE_C_FLAGS}")
    endif()
else()
    set(CMAKE_C_STANDARD 99)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W2")
else()
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3 -DDEBUG")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Werror -fomit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-field-initializers -Wno-unused-parameter -Wno-unused-function")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-variable -Wno-unused-value -Wno-unused-result -Wno-missing-braces")
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

set(u_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(u_SOURCE_DIR  ${CMAKE_CURRENT_LIST_DIR}/src)
set(u_TEST_DIR    ${CMAKE_CURRENT_LIST_DIR}/test)

file(GLOB u_HEADERS ${u_HEADERS} ${u_INCLUDE_DIR}/*.h)
file(GLOB u_SOURCES ${u_SOURCES} ${u_SOURCE_DIR}/*.c)

include_directories(${u_INCLUDE_DIR})
add_library(${PROJECT_NAME} STATIC ${u_SOURCES} ${u_HEADERS})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC winmm)
else()
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC m)
endif()

enable_testing()
add_subdirectory(${u_TEST_DIR})
