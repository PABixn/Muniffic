# msdf-atlas-gen/CMakeLists.txt

cmake_minimum_required(VERSION 3.16)
add_subdirectory(msdfgen)
project(msdf-atlas-gen LANGUAGES CXX)

add_library(msdf-atlas-gen STATIC)

set_target_properties(msdf-atlas-gen PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

#if(MSVC)
#    target_compile_options(msdf-atlas-gen PRIVATE /w)
#else()
#    target_compile_options(msdf-atlas-gen PRIVATE -w)
#endif()

# Output directories for binaries and object files
set_target_properties(msdf-atlas-gen PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/msdf-atlas-gen-bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/msdf-atlas-gen-bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/msdf-atlas-gen-bin"
    OUTPUT_NAME "msdf-atlas-gen"
)

target_link_directories(msdf-atlas-gen INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/msdf-atlas-gen/msdf-atlas-gen-bin/${CMAKE_BUILD_TYPE}")

# Source files
file(GLOB_RECURSE MSDF-ATLAS-GEN_SOURCE "msdf-atlas-gen/*.h" "msdf-atlas-gen/*.hpp" "msdf-atlas-gen/*.cpp")

target_sources(msdf-atlas-gen PRIVATE ${MSDF-ATLAS-GEN_SOURCE})

# Include directories
target_include_directories(msdf-atlas-gen PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/msdf-atlas-gen"
    "${CMAKE_CURRENT_SOURCE_DIR}/msdfgen"
    "${CMAKE_CURRENT_SOURCE_DIR}/msdfgen/include"
)

# Link msdfgen library
target_link_libraries(msdf-atlas-gen PRIVATE msdfgen)

# Platform-specific settings
if(WIN32)
    set(CMAKE_SYSTEM_VERSION "latest")
endif()
