project "lual"
    kind "StaticLib"
    language "C"
    targetdir(bin_directory)
    objdir(obj_directory)

    files {
        "lua/**.c",
        "lua/**.h",
    }
    removefiles { "lua/lua.c", "lua/luac.c", "lua/onelua.c" }

    filter "configurations:debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"
