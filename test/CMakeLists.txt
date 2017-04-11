enable_testing()

unset(CMAKE_RUNTIME_OUTPUT_DIRECTORY)

include_directories(${CMAKE_CURRENT_LIST_DIR})

file(GLOB adt_TST_DIRS ${adt_TST_DIRS} *)
foreach(test ${adt_TST_DIRS})
    if(IS_DIRECTORY ${test})
        get_filename_component(test_NAME ${test}/main.c PATH)
        get_filename_component(test_NAME ${test_NAME} NAME)
        if(NOT ${test_NAME} STREQUAL test)
            add_executable(${test_NAME} ${test}/main.c)
            add_test(test_${test_NAME} ${test_NAME})
            target_link_libraries(${test_NAME} ${PROJECT_NAME})
        endif()
    endif()
endforeach()
