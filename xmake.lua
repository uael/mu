set_project("libu")
set_version("0.0.1")

if is_mode("r") then
    set_symbols("hidden")
    set_optimize("fastest")
    set_strip("all")
end

if is_mode("d") then
    set_symbols("debug")
    set_optimize("none")
end

target("u")
    set_default(true)
    set_languages("c99")
    set_kind("$(kind)")
    set_headerdir("$(buildir)/include/u")
    add_headers("include/(u/*.h)")
    add_includedirs("$(buildir)", "include")
    add_files("src/*.c")
    add_defines("U_COMPILE=1")

add_subdirs("vendor", "test")
