#ifndef GE_VULKAN_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <cassert>

namespace ge::vulkan
{
    std::vector<VkExtensionProperties> get_available_extensions()
    {
        uint32_t count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

        std::vector<VkExtensionProperties> vkExtensions(count);
        vkEnumerateInstanceExtensionProperties(nullptr, &count, vkExtensions.data());

        return vkExtensions;
    }

    bool extension_available(const std::string& extensionName, std::optional<uint32_t> optVersion = std::nullopt)
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

    void create_instance(SDL_Window* window, VkInstance& vkInstance)
    {
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
    
        uint32_t sdlExtensionCount = 0;
        assert( SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, NULL) );
        std::vector<char*> reqSdlExt(sdlExtensionCount);
        assert( SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, const_cast<const char**>( reqSdlExt.data() ) ) );

        for( const auto& ext : reqSdlExt )
        {
            std::cout << "SDL Ext: " << ext << " is avaiable \"" << (extension_available(ext) ? "true" : "false") << "\""  << std::endl;

        }
    
        createInfo.enabledExtensionCount = sdlExtensionCount;
        createInfo.ppEnabledExtensionNames = (const char**)reqSdlExt.data();
        createInfo.enabledLayerCount = 0;
    
        VkResult vkResult = vkCreateInstance(&createInfo, nullptr, &vkInstance);
        assert( vkResult == VK_SUCCESS );
    }

}

#endif
