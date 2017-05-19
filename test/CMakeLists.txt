file(GLOB ctest_SOURCES ${ctest_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/*.c)

foreach(ctest_src ${ctest_SOURCES})
    get_filename_component(ctest_name ${ctest_src} NAME_WE)
    add_executable(${ctest_name}_test ${ctest_src})
    add_dependencies(${ctest_name}_test ${PROJECT_NAME})
    target_link_libraries(${ctest_name}_test ${PROJECT_NAME})
    add_test(${ctest_name} ${ctest_name}_test)
endforeach()
