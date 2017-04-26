include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
    endif()
else()
    set(CMAKE_CXX_STANDARD 11)
endif()

if(WIN32)
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
    endforeach()
endif()

file(GLOB check_SOURCES ${check_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/*.cc)

add_executable(tests ${check_SOURCES})
add_dependencies(tests ${PROJECT_NAME} gtest gtest_main)
target_link_libraries(tests gtest gtest_main)
target_link_libraries(tests ${PROJECT_NAME})

add_test(check tests)
