target("test")
    set_default(false)
    set_languages("cxx11")
    set_kind("binary")
    add_deps("u", "gtest", "gtest_main")
    add_links("pthread", "gtest", "gtest_main", "u")
    add_includedirs("../include")
    add_cxflags("-pthread")
    add_ldflags("-pthread")
    add_linkdirs("$(buildir)")
    add_includedirs("../vendor/gtest/googletest/include", "../vendor/gtest/googletest")
    add_files("*.cc")

task("check")
    on_run(function ()
        import("core.project.task")
        task.run("run", {target = "test"})
    end)
    set_menu {
        usage = "xmake check"
    ,   description = "Run tests !"
    ,   options = {}
    }
