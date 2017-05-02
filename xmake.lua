set_project("libu")
set_languages("c99")

if is_mode("r") then
    set_symbols("hidden")
    set_optimize("fastest")
    set_strip("all")
end

if is_mode("d") then
    set_symbols("debug")
    set_optimize("none")
end

-- libsvx.a & libsvx.so
target("u")
    set_kind("$(kind)")
    add_includedirs("include")
    add_headers("include/*.h")
    add_files("src/*.c")

set_languages("c++11")

target("gtest")
    set_kind("static")
    add_links("pthread")
    add_cxflags("-pthread")
    add_ldflags("-pthread")
    add_includedirs("vendor/gtest/googletest/include", "vendor/gtest/googletest")
    add_files("vendor/gtest/googletest/src/gtest-all.cc")

target("gtest_main")
    set_kind("static")
    add_links("pthread", "gtest")
    add_cxflags("-pthread")
    add_ldflags("-pthread")
    add_includedirs("vendor/gtest/googletest/include", "vendor/gtest/googletest")
    add_files("vendor/gtest/googletest/src/gtest_main.cc")

target("test")
    set_default(true)
    set_kind("binary")
    add_deps("u", "gtest", "gtest_main")
    add_links("pthread", "gtest", "gtest_main", "u")
    add_includedirs("include")
    add_cxflags("-std=c++11", "-pthread")
    add_ldflags("-std=c++11", "-pthread")
    add_linkdirs("$(buildir)")
    add_includedirs("vendor/gtest/googletest/include", "vendor/gtest/googletest")
    add_files("test/vector.cc")
