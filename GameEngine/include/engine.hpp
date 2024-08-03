#ifndef GE_ENGINE_H_ 
#define GE_ENGINE_H_

#include <cstdint>
#include <string>

#include <SDL2/SDL.h>
#include <utility/vec2.hpp>

namespace ge
{
    
    class Engine 
    {
        SDL_Window* m_sdlWindow = nullptr;
        SDL_GLContext m_sdlGLContext;

        bool m_isRunning = false;

    public:
        Engine(const Vec2<uint32_t>& resolution, const std::string& title);
        ~Engine();

        bool run();
    };

}

#endif
