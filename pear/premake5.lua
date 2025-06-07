project "pear"
    kind "StaticLib"
    language "C"

    debugdir(bin_directory)
    targetdir(bin_directory)
    objdir(obj_directory)

    files {
        "src/**.c",
        "include/**.h", 
        "../3rdparty/stb/stb_image.c",
        "../3rdparty/cgltf/cgltf.c",
    }

    includedirs {
        "include",
        "../3rdparty/glfw/include",
        "../3rdparty/sokol",
        "../3rdparty/cglm/include",
        "../3rdparty/stb",
        "../3rdparty/cgltf",
        "../3rdparty/lua",
    }

    prebuildcommands {
        "mkdir -p include/shaders",
        "../3rdparty/sokol-tools-bin/bin/linux/sokol-shdc --input shaders/shader.glsl --output include/shaders/shader.h --slang glsl430",
    }

    filter "configurations:debug"
        defines { "PEAR_DEBUG" }
        symbols "On"

    filter "configurations:release"
        defines { "PEAR_RELEASE" }
        optimize "On"

    filter "system:linux"
        defines { "PEAR_LINUX" }
