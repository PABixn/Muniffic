# Box2D/CMakeLists.txt

project(Box2D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Box2D STATIC)

# Output directories for binary and intermediate files
set_target_properties(Box2D PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Box2D"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Box2D"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Box2D"
    OUTPUT_NAME "Box2D"
)

# Source files
file(GLOB_RECURSE BOX2D_SRC "src/*.cpp" "src/*.h")
file(GLOB_RECURSE BOX2D_INCLUDE "include/*.h")

target_sources(Box2D PRIVATE ${BOX2D_SRC} ${BOX2D_INCLUDE})

# Include directories
target_include_directories(Box2D PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if (WIN32)
    target_compile_definitions(Box2D PRIVATE _CRT_SECURE_NO_WARNINGS) # Disable security warnings on MSVC
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Box2D)
endif()

# Configurations
target_compile_options(Box2D PRIVATE
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:Dist>:-O3 -DNDEBUG>
)

# Enable or disable symbols
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Box2D PRIVATE BOX2D_DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif(CMAKE_BUILD_TYPE STREQUAL "Dist")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS_DIST "${CMAKE_CXX_FLAGS_DIST} -DNDEBUG")
endif()

