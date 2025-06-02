project "glfw"
    kind "StaticLib"
    language "C"
    targetdir(bin_directory)
    objdir(obj_directory)

    files {
        "glfw/include/GLFW/glfw3.h",
        "glfw/include/GLFW/glfw3native.h",
        "glfw/src/context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/internal.h",
        "glfw/src/mapping.h",
        "glfw/src/mapping.h.in",
        "glfw/src/monitor.c",
        "glfw/src/null_init.c",
        "glfw/src/null_joystick.c",
        "glfw/src/null_joystick.h",
        "glfw/src/null_monitor.c",
        "glfw/src/null_platform.h",
        "glfw/src/null_window.c",
        "glfw/src/platform.c",
        "glfw/src/platform.h",
        "glfw/src/vulkan.c",
        "glfw/src/window.c",
    }

    filter "system:linux"
        systemversion "latest"
        pic "On"

        defines "_GLFW_X11"

        files {
            "glfw/src/egl_context.c",
            "glfw/src/glx_context.c",
            "glfw/src/linux_joystick.h",
            "glfw/src/linux_joystick.c",
            "glfw/src/osmesa_context.c",
            "glfw/src/posix_module.c",
            "glfw/src/posix_poll.c",
            "glfw/src/posix_poll.h",
            "glfw/src/posix_thread.c",
            "glfw/src/posix_thread.h",
            "glfw/src/posix_time.c",
            "glfw/src/posix_time.h",
            "glfw/src/x11_init.c",
            "glfw/src/x11_monitor.c",
            "glfw/src/x11_platform.h",
            "glfw/src/x11_window.c",
            "glfw/src/xkb_unicode.c",
            "glfw/src/xkb_unicode.h",
        }

    filter "configurations:debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:release"
        runtime "Release"
        optimize "Speed"
        symbols "Off"
