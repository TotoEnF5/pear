project "runtime"
    kind "ConsoleApp"
    language "C"

    debugdir(bin_directory)
    targetdir(bin_directory)
    objdir(obj_directory)

    files {
        "src/**.c",
        "include/**.h",
    }

    includedirs {
        "include",
        "../pear/include",
        "../3rdparty/cglm/include",
        "../3rdparty/sokol",
    }

    links {
        "pear",
        "m",
        "glfw",
        "lua",
        "GL",
    }

    filter "configurations:debug"
        defines { "PEAR_DEBUG" }
        symbols "On"

    filter "configurations:release"
        defines { "PEAR_RELEASE" }
        optimize "On"
