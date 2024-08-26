#ifndef GE_VULKAN_HPP

#include <utility/logging.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <cassert>

namespace ge::vulkan
{
    
    const std::vector<const char*> VALIDATION_LAYERS = {
        "VK_LAYER_KHRONOS_validation"
    };

    #ifdef _DEBUG
        const bool ENABLE_VALIDATION_LAYERS = true;
    #else
        const bool ENABLE_VALIDATION_LAYERS = false;
    #endif

    bool check_validation_layer_support()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* validationLayer : VALIDATION_LAYERS) 
        {
            bool layerFound = false;
        
            for (const auto& layerProperties : availableLayers) 
            {
                if (strcmp(validationLayer, layerProperties.layerName) == 0) 
                {
                    logger.status("Vulkan", std::format("Validation layer \"{}\" supported.", validationLayer));
                    layerFound = true;
                    break;
                }
            }
        
            if (!layerFound)
            {
                logger.error("Vulkan", std::format("Validation layer \"{}\" not supported.", validationLayer));
                return false;
            }
        }

        logger.status("Vulkan", "Needed validation layers are supported.");

        return true;
    }

    std::vector<const char*> get_required_extensions(SDL_Window* window)
    {
        uint32_t sdlExtensionCount = 0;
        assert( SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, NULL) );
        std::vector<const char*> requiredExtensions(sdlExtensionCount);
        assert( SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, requiredExtensions.data() ) );
        
        if( ENABLE_VALIDATION_LAYERS )
        {
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // Extension for VK_EXT_debug_utils
        }
       
        return requiredExtensions;
    }

    std::vector<VkExtensionProperties> get_available_extensions()
    {
        uint32_t count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

        std::vector<VkExtensionProperties> vkExtensions(count);
        vkEnumerateInstanceExtensionProperties(nullptr, &count, vkExtensions.data());

        return vkExtensions;
    }

    bool is_extension_available(const std::string& extensionName, std::optional<uint32_t> optVersion = std::nullopt)
    {
        // TODO: Potentially check case insensetive
        const static std::vector<VkExtensionProperties> AV_EXTENSIONS = get_available_extensions();
        
        for( const auto& ext : AV_EXTENSIONS )
        {
            const std::string availableExt(ext.extensionName); 
            if( availableExt == extensionName )
            {
                if( optVersion.has_value() )
                {
                    if( optVersion.value() >= ext.specVersion )
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                    
                return true;
            }
        }

        return false;
    }

    bool create_instance(SDL_Window* window, VkInstance& vkInstance)
    {
        if( ENABLE_VALIDATION_LAYERS && !check_validation_layer_support() )
        {
            logger.error("Vulkan", "Validation layers enabled but not supported."); 
            return false;
        }

        std::vector<const char*> requiredExtensions = get_required_extensions(window);
        uint32_t countAvailable = 0;
        for( const auto& ext : requiredExtensions)
        {
            bool isAvailable = is_extension_available(ext);
            if(isAvailable)
            {
                countAvailable += 1;
            }

            logger.status("Vulkan", std::format("Required Vulkan extension: \"{}\" available \"{}\"", ext, (isAvailable ? "true" : "false")));
        }

        if( countAvailable >= requiredExtensions.size() )
        {
            logger.status("Vulkan", "Vulkan required extensions was all available!");
        }
        else
        {
            logger.error("Vulkan", "Vulkan required extensions not all available.");
            return false;
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
    
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
    
        createInfo.enabledExtensionCount = static_cast<uint32_t>( requiredExtensions.size() );
        createInfo.ppEnabledExtensionNames = (const char**)requiredExtensions.data();

        if( ENABLE_VALIDATION_LAYERS )
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>( VALIDATION_LAYERS.size() );
            createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }
    
        VkResult vkResult = vkCreateInstance(&createInfo, nullptr, &vkInstance);
        assert( vkResult == VK_SUCCESS );

        return true;
    }

}

#endif
