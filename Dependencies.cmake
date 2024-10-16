# Get Vulkan SDK path from environment variable
set(VULKAN_SDK $ENV{VULKAN_SDK})

# Include directories
set(IncludeDir_stb_image "${CMAKE_SOURCE_DIR}/Engine/vendor/stb_image")
set(IncludeDir_yaml_cpp "${CMAKE_SOURCE_DIR}/Engine/vendor/yaml-cpp/include")
set(IncludeDir_Box2D "${CMAKE_SOURCE_DIR}/Engine/vendor/Box2D/include")
set(IncludeDir_filewatch "${CMAKE_SOURCE_DIR}/Engine/vendor/filewatch")
set(IncludeDir_GLFW "${CMAKE_SOURCE_DIR}/Engine/vendor/GLFW/include")
set(IncludeDir_Glad "${CMAKE_SOURCE_DIR}/Engine/vendor/Glad/include")
set(IncludeDir_ImGui "${CMAKE_SOURCE_DIR}/Engine/vendor/Imgui")
set(IncludeDir_ImGuizmo "${CMAKE_SOURCE_DIR}/Engine/vendor/ImGuizmo")
set(IncludeDir_glm "${CMAKE_SOURCE_DIR}/Engine/vendor/glm")
set(IncludeDir_entt "${CMAKE_SOURCE_DIR}/Engine/vendor/entt/include")
set(IncludeDir_mono "${CMAKE_SOURCE_DIR}/Engine/vendor/mono/include")
set(IncludeDir_shaderc "${CMAKE_SOURCE_DIR}/Engine/vendor/shaderc/include")
set(IncludeDir_SPIRV_Cross "${CMAKE_SOURCE_DIR}/Engine/vendor/SPIRV-Cross")
set(IncludeDir_VulkanSDK "${VULKAN_SDK}/Include")
set(IncludeDir_msdfgen "${CMAKE_SOURCE_DIR}/Engine/vendor/msdf-atlas-gen/msdfgen")
set(IncludeDir_msdf_atlas_gen "${CMAKE_SOURCE_DIR}/Engine/vendor/msdf-atlas-gen/msdf-atlas-gen")

# Library directories
set(LibraryDir_VulkanSDK "${VULKAN_SDK}/Lib")


set(LibraryDir_python 
"$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/Engine/vendor/python/lib/Debug>"
"$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:Dist>>:${CMAKE_SOURCE_DIR}/Engine/vendor/python/lib/Release>"
)
# Libraries
set(Library_mono 
"$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/Engine/vendor/mono/lib/Debug/libmono-static-sgen.lib>"
"$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:Dist>>:${CMAKE_SOURCE_DIR}/Engine/vendor/mono/lib/Release/libmono-static-sgen.lib>"
)
set(Library_Vulkan "${LibraryDir_VulkanSDK}/vulkan-1.lib")
set(Library_VulkanUtils "${LibraryDir_VulkanSDK}/VkLayer_utils.lib")

set(Library_ShaderC_Debug "${LibraryDir_VulkanSDK}/shaderc_sharedd.lib")
set(Library_SPIRV_Cross_Debug "${LibraryDir_VulkanSDK}/spirv-cross-cored.lib")
set(Library_SPIRV_Cross_GLSL_Debug "${LibraryDir_VulkanSDK}/spirv-cross-glsld.lib")
set(Library_SPIRV_Tools_Debug "${LibraryDir_VulkanSDK}/SPIRV-Toolsd.lib")

set(Library_ShaderC_Release "${LibraryDir_VulkanSDK}/shaderc_shared.lib")
set(Library_SPIRV_Cross_Release "${LibraryDir_VulkanSDK}/spirv-cross-core.lib")
set(Library_SPIRV_Cross_GLSL_Release "${LibraryDir_VulkanSDK}/spirv-cross-glsl.lib")

# Windows Libraries
set(Library_WinSock "Ws2_32.lib")
set(Library_WinMM "Winmm.lib")
set(Library_WinVersion "Version.lib")
set(Library_BCrypt "Bcrypt.lib")
