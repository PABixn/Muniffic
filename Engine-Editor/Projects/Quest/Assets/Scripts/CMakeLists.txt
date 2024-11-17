cmake_minimum_required(VERSION 3.8)

set(CMAKE_SHARED_LINKER_FLAGS_DIST "")
add_subdirectory("${CMAKE_BINARY_DIR}/../../../../../Muniffic-ScriptCore" "${CMAKE_BINARY_DIR}/../../../../../Muniffic-ScriptCore")
#set(PROJECT_NAME "Example" CACHE STRING "name of project")
set(MyProject_BUILD_TYPES Debug RelWithDebInfo Release Dist)
get_property(multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
set(CMAKE_CONFIGURATION_TYPES "${MyProject_BUILD_TYPES}" CACHE STRING "list of supported configuration types" FORCE)

project(${PROJECT_NAME} LANGUAGES CSharp)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x86_64")
else()
    set(ARCHITECTURE "x86")
endif()

# set(MunifficRootDir "../../../../../")

# Set the output directories for binaries and intermediate files
add_library(${PROJECT_NAME} SHARED)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY   
"$<IF:$<CONFIG:Debug>,${CMAKE_CURRENT_BINARY_DIR}/Binaries/Debug,
 $<IF:$<CONFIG:Release>,${CMAKE_BINARY_DIR}/Binaries/Release,
 $<IF:$<CONFIG:RelWithDebInfo>,${CMAKE_BINARY_DIR}/Binaries/RelWithDebInfo,
 $<IF:$<CONFIG:Dist>,${CMAKE_BINARY_DIR}/Binaries/Dist,
 ${CMAKE_CURRENT_BINARY_DIR}/Binaries/Debug>>>>"
)

set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY    
"$<IF:$<CONFIG:Debug>,${CMAKE_CURRENT_BINARY_DIR}/Binaries/Debug,
 $<IF:$<CONFIG:Release>,${CMAKE_BINARY_DIR}/Binaries/Release,
 $<IF:$<CONFIG:RelWithDebInfo>,${CMAKE_BINARY_DIR}/Binaries/RelWithDebInfo,
 $<IF:$<CONFIG:Dist>,${CMAKE_BINARY_DIR}/Binaries/Dist,
 ${CMAKE_CURRENT_BINARY_DIR}/Binaries/Debug>"
)

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
"$<IF:$<CONFIG:Debug>,${CMAKE_CURRENT_BINARY_DIR}/Binaries/Debug,
 $<IF:$<CONFIG:Release>,${CMAKE_BINARY_DIR}/Binaries/Release,
 $<IF:$<CONFIG:RelWithDebInfo>,${CMAKE_BINARY_DIR}/Binaries/RelWithDebInfo,
 $<IF:$<CONFIG:Dist>,${CMAKE_BINARY_DIR}/Binaries/Dist,
 ${CMAKE_CURRENT_BINARY_DIR}/Binaries/Debug>"
)

set_target_properties(${PROJECT_NAME} PROPERTIES OBJECT_OUTPUT_DIRECTORY
"$<IF:$<CONFIG:Debug>,${CMAKE_CURRENT_BINARY_DIR}/Intermediates/Debug,
 $<IF:$<CONFIG:Release>,${CMAKE_BINARY_DIR}/Intermediates/Release,
 $<IF:$<CONFIG:RelWithDebInfo>,${CMAKE_BINARY_DIR}/Intermediates/RelWithDebInfo,
 $<IF:$<CONFIG:Dist>,${CMAKE_BINARY_DIR}/Intermediates/Dist,
 ${CMAKE_CURRENT_BINARY_DIR}/Intermediates/Debug>"
)


# Define the target framework
# set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2")

# Add source files
file(GLOB_RECURSE SOURCES
    "Source/*.cs"
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

add_dependencies(${PROJECT_NAME} Muniffic-ScriptCore)

# Debug configuration
set(CMAKE_CSharp_FLAGS_DEBUG 
"$<$<CONFIG:Debug>:/debug:full>"
"$<&<CONFIG:RelWithDebInfo>:/optimize /debug:full>"
"$<$<CONFIG:Release>:/optimize>"
"$<$<CONFIG:Dist>:/optimize+ /debug:none>"
)

target_compile_options(${PROJECT_NAME} PRIVATE 
    "$<$<CONFIG:Debug>:/debug:full>"
    "$<$<CONFIG:RelWithDebInfo>:/optimize /debug:full>"
    "$<$<CONFIG:Release>:/optimize>"
    "$<$<CONFIG:Dist>:/optimize+ /debug:none>"
)
