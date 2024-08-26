#include <engine.hpp>

#include <iostream>
#include <stdexcept>
#include <format>
#include <chrono>

#include <utility/logging.hpp>
#include <vulkan/vulkan.hpp>

namespace ge
{
    bool initSDL()
    {
        static bool hasInitialized = false;
        if(!hasInitialized)
        {
            int result = SDL_InitSubSystem(SDL_INIT_EVERYTHING);
            if(result == 0)
            {
                return true;
            }
            else
            {
                std::string strError(SDL_GetError());
                if(!strError.empty())
                {
                    logger.error("initSDL", strError);
                }
                else
                {
                    logger.error("initSDL", "Unknown error on SDL_InitSubSystem.");
                }

                return false;
            }
        }

        return true;
    }

    Engine::Engine(const Vec2<uint32_t>& resolution, const std::string& title)
    {
        initSDL();

        m_sdlWindow = SDL_CreateWindow( 
            title.c_str(), 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            resolution.x, resolution.y, 
            SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN
        );

        if(!m_sdlWindow)
        {
            throw std::runtime_error( std::format("ERROR: SDL Create Window => {}", SDL_GetError()) );
        }

        assert( true == ge::vulkan::create_instance(m_sdlWindow, m_vkInstance) );
    }

    Engine::~Engine()
    {
        vkDestroyInstance(m_vkInstance, nullptr);

        SDL_DestroyWindow(m_sdlWindow);
        m_sdlWindow = nullptr;

        SDL_Quit();
    }

    bool Engine::run()
    {
        if(m_isRunning) return false; // already running
                                      
        m_isRunning = true;

        std::chrono::time_point<std::chrono::high_resolution_clock> currentFrameTime, lastFrameTime;
        double accumilatedTime = 0.0;
        uint64_t accumilatedFrames = 0;
        SDL_Event e;
        while( m_isRunning )
        {
            currentFrameTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::chrono::seconds::period> deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;
            accumilatedTime = accumilatedTime + deltaTime.count();
            accumilatedFrames += 1;

            if( accumilatedTime >= 5 )
            {
                accumilatedTime = 0.0;
                logger.status("Engine", std::format("Current frame time: \"{}\" estimated FPS: \"{}\".", deltaTime.count(), accumilatedFrames / 5));
                logger.flush();
            }

            while( SDL_PollEvent( &e ) != 0 )
            {
                if( e.type == SDL_QUIT )
                {
                    m_isRunning = false;
                }
            }

            // TODO: Game Update (physics, logic etc)
            // TODO: Render Update (rendering)

            // TODO: Swap window vulkan(swap chain)
        }

        return true;
    }

}
