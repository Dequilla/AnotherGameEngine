#ifndef GE_ENGINE_H_ 
#define GE_ENGINE_H_

#include <cstdint>
#include <string>

#include <SDL2/SDL.h>
#include <vulkan/vulkan_core.h>

#include <utility/vec2.hpp>

namespace ge
{
    
    class Engine 
    {
        SDL_Window* m_sdlWindow = nullptr;
        VkInstance m_vkInstance;

        bool m_isRunning = false;

    public:
        Engine(const Vec2<uint32_t>& resolution, const std::string& title);
        ~Engine();

        bool run();
    };

}

#endif
