# This file is used the correctly locate Vulkan SDK and choose the best suited version
# It's assumed the Vulkan SDKs are located in C:/VulkanSDK. If that is incorrect, change the next line appropriately
set(VULKAN_SDK_LOCATION "C:/VulkanSDK")


# ---------------------------------------------------------------
# Get all versions
file(GLOB ALL_FILES "${VULKAN_SDK_LOCATION}/*")

# Select the newest Vulkan SDK version
set(LATEST_VERSION "0.0.0.0")
foreach(FILE ${ALL_FILES})
    get_filename_component(DIR_NAME ${FILE} NAME)
    if("${DIR_NAME}" VERSION_GREATER "${LATEST_VERSION}")
        set(LATEST_VERSION ${DIR_NAME})
    endif()
endforeach()

set(VULKAN_SDK_LOCATION "${VULKAN_SDK_LOCATION}/${LATEST_VERSION}" CACHE INTERNAL "${VULKAN_SDK_LOCATION}/${LATEST_VERSION}")
message(STATUS "VULKAN_SDK_LOCATION is set to: ${VULKAN_SDK_LOCATION}")