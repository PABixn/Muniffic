# GLFW/CMakeLists.txt

cmake_minimum_required(VERSION 3.16)
project(GLFW LANGUAGES C)

add_library(GLFW STATIC)

# Output directories for binaries and object files
set_target_properties(GLFW PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/GLFW"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/GLFW"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/GLFW"
    OUTPUT_NAME "GLFW"
)

# Source files
set(GLFW_FILES
    "include/GLFW/glfw3.h"
    "include/GLFW/glfw3native.h"
    #"src/glfw_config.h"
    "src/context.c"
    "src/init.c"
    "src/input.c"
    "src/monitor.c"
    "src/null_init.c"
    "src/null_joystick.c"
    "src/null_monitor.c"
    "src/null_window.c"
    "src/platform.c"
    "src/vulkan.c"
    "src/window.c"
)

target_sources(GLFW PRIVATE ${GLFW_FILES})

# Platform-specific source files and defines
if(UNIX AND NOT APPLE) # Linux
    target_sources(GLFW PRIVATE
        "src/x11_init.c"
        "src/x11_monitor.c"
        "src/x11_window.c"
        "src/xkb_unicode.c"
        "src/posix_module.c"
        "src/posix_time.c"
        "src/posix_thread.c"
        "src/glx_context.c"
        "src/egl_context.c"
        "src/osmesa_context.c"
        "src/linux_joystick.c"
    )
    target_compile_definitions(GLFW PRIVATE _GLFW_X11)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

elseif(APPLE) # macOS
    target_sources(GLFW PRIVATE
        "src/cocoa_init.m"
        "src/cocoa_monitor.m"
        "src/cocoa_window.m"
        "src/cocoa_joystick.m"
        "src/cocoa_time.c"
        "src/nsgl_context.m"
        "src/posix_thread.c"
        "src/posix_module.c"
        "src/osmesa_context.c"
        "src/egl_context.c"
    )
    target_compile_definitions(GLFW PRIVATE _GLFW_COCOA)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

elseif(WIN32) # Windows
    target_sources(GLFW PRIVATE
        "src/win32_init.c"
        "src/win32_joystick.c"
        "src/win32_module.c"
        "src/win32_monitor.c"
        "src/win32_time.c"
        "src/win32_thread.c"
        "src/win32_window.c"
        "src/wgl_context.c"
        "src/egl_context.c"
        "src/osmesa_context.c"
    )
    target_compile_definitions(GLFW PRIVATE _GLFW_WIN32 _CRT_SECURE_NO_WARNINGS)
    set(CMAKE_SYSTEM_VERSION "latest")
endif()


# Symbols for different configurations
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(GLFW PRIVATE GLFW_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Dist")
    if(CMAKE_BUILD_TYPE STREQUAL "Dist")
        set(CMAKE_C_FLAGS_DIST "${CMAKE_C_FLAGS_DIST} -DNDEBUG")
        set(CMAKE_CXX_FLAGS_DIST "${CMAKE_CXX_FLAGS_DIST} -DNDEBUG")
    endif()
endif()

