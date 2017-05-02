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

add_subdirs("vendor", "test")
