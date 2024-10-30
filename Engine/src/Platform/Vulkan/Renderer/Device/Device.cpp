#include "Device.h"
#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <set>
#include <map>
// Helpers

    // Debug messenger related functions
    VkResult DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT pDebugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, pDebugMessenger, pAllocator);
            return VK_SUCCESS;
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    // Extensions
    std::vector<const char*> eg::Device::getRequiredExtensions() const
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        //extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

        return extensions;
    }

    // Physical devices
    bool checkDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions((eg::deviceExtensions).begin(), (eg::deviceExtensions).end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    uint32_t eg::RateDevice(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        int score = 0;

        // Application can't function without geometry shaders
        if (!deviceFeatures.geometryShader) {
            return 0;
        }
        // Vulkan needs queues
        QueueFamilyIndices indices = Queue::GetQueueFamiliesIndices(device, surface);
        if (!indices.isComplete())
        {
            return 0;
        }
        // Check the device extensions App requires
        if (!checkDeviceExtensionSupport(device))
        {
            return 0;
        }
        // TO-DO Vulkan 
        // make it work:
        // 
        // Check if device supports swap chains
        /*SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
        if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty()) {
            return 0;
        }*/
        // Application requires anisotropy
        if (!deviceFeatures.samplerAnisotropy)
        {
            return 0;
        }


        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        score += deviceProperties.limits.maxImageDimension2D;

        return score;
    }




// Basic functions

void eg::Device::init(GLFWwindow* glfwWindow)
    {
        createVulkanInstance();
        setupDebugMessanger();
        CreateSurface(glfwWindow);
        PickPhysicalDevice();
        CreateLogicalDevice();
    }

void eg::Device::cleanUp()
    {
        vkDestroyDevice(m_LogicalDevice, nullptr);
        vkDestroySurfaceKHR(m_VulkanInstance, m_Surface, nullptr);
        if (enableValidationLayers)DestroyDebugUtilsMessengerEXT(m_VulkanInstance, (*m_DebugMessenger.get()), nullptr);
        vkDestroyInstance(m_VulkanInstance, nullptr);
    }


// Components

    // Instance
    void eg::Device::createVulkanInstance()
    {
        // vk validation layers setup
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available :(");
        }

        // vk instance setup
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Muniffic";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Muni";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // vk extensions setup
        std::vector<const char*> requiredExtensions = getRequiredExtensions();

        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        // vk validation layer setup
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
            createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }


        // vk instance creation
        VkResult res = vkCreateInstance(&createInfo, nullptr, &m_VulkanInstance);
        
        if (res != VK_SUCCESS) {
            std::cout << "not silli: " << res << std::endl;
            throw std::runtime_error("failed to create instance :(");
        }
    }

    // Debug messenger
    void eg::Device::setupDebugMessanger()
    {
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = &debugCallback;
        createInfo.pUserData = nullptr;

        if (CreateDebugUtilsMessengerEXT(m_VulkanInstance, &createInfo, nullptr, m_DebugMessenger.get()) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    bool eg::Device::checkValidationLayerSupport() const
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : m_ValidationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    // Physical Device
    void eg::Device::PickPhysicalDevice()
    {
        m_PhysicalDevice = VK_NULL_HANDLE;
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceCount, nullptr);
        EG_ASSERT(deviceCount != 0, "Failed to find GPUs with Vulkan support :(");
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceCount, devices.data());

        //selecting the best GPU we have
        std::multimap<int, VkPhysicalDevice> candidates;

        for (const auto& device : devices) {
            int score = RateDevice(device, m_Surface);
            candidates.insert(std::make_pair(score, device));
        }

        if (candidates.rbegin()->first > 0) {
            m_PhysicalDevice = candidates.rbegin()->second;
            #ifndef NDEBUG

            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(m_PhysicalDevice, &deviceProperties);
            EG_TRACE("Choosen physical device: {0}", deviceProperties.deviceName);
            #endif // !NDEBUG

        }

        else {
            EG_ASSERT(false, "failed to find a suitable GPU :(");
        }

    }

    // Local Device
    void eg::Device::CreateLogicalDevice()
    {
        // queue setup


        #ifndef NDEBUG
        showQueueFamilies(m_PhysicalDevice);
        #endif // !NDEBUG

        Queue::GetQueueFamiliesIndices(m_PhysicalDevice, m_Surface, true);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::vector<float>* prorities = Queue::PopulateQueuesCreationInfo(queueCreateInfos);

        // logical device setup
        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
            createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        // queue getting
        vkGetDeviceQueue(m_LogicalDevice, Queue::GetQueueFamiliesIndices(m_PhysicalDevice, m_Surface).graphicsFamily.value(), 0, (m_GraphicsQueue.GetNativeQueue()).get());
        vkGetDeviceQueue(m_LogicalDevice, Queue::GetQueueFamiliesIndices(m_PhysicalDevice, m_Surface).presentFamily.value(), 0, (m_PresentQueue.GetNativeQueue()).get());

        prorities->clear();
        delete prorities;
        prorities = nullptr;

    }

    void eg::Device::CreateSurface(GLFWwindow* nativeWindow)
    {
         if (glfwCreateWindowSurface(m_VulkanInstance, nativeWindow, nullptr, &m_Surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }
