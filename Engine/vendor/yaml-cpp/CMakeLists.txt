# yaml-cpp/CMakeLists.txt

cmake_minimum_required(VERSION 3.16)
project(yaml-cpp LANGUAGES CXX)

add_library(yaml-cpp STATIC)

set_target_properties(yaml-cpp PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Output directories for binaries and object files
set_target_properties(yaml-cpp PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp"
    OUTPUT_NAME "yaml-cpp"
)

file(GLOB_RECURSE yaml-cpp_SOURCE "src/*.h" "src/*.cpp" "include/*.h")

target_sources(yaml-cpp PRIVATE ${yaml-cpp_SOURCE})

# Include directories
target_include_directories(yaml-cpp PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Platform-specific settings
if(WIN32)
    set(CMAKE_SYSTEM_VERSION "latest")
    set_target_properties(yaml-cpp PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>")
elseif(UNIX AND NOT APPLE) # Linux
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_SYSTEM_VERSION "latest")
endif()

# Build configuration settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(yaml-cpp PRIVATE DEBUG)
endif()