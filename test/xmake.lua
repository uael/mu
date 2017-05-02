set_languages("c++11")

target("test")
    set_kind("binary")
    add_deps("u", "gtest", "gtest_main")
    add_links("pthread", "gtest", "gtest_main", "u")
    add_includedirs("../include")
    add_cxflags("-std=c++11", "-pthread")
    add_ldflags("-std=c++11", "-pthread")
    add_linkdirs("$(buildir)")
    add_includedirs("../vendor/gtest/googletest/include", "../vendor/gtest/googletest")
    add_files("vector.cc")

task("check")
    on_run(function ()
        import("core.project.task")
        task.run("run", "test")
    end)
    set_menu {
        usage = "xmake check"
    ,   description = "Run tests !"
    ,   options = {}
    }
