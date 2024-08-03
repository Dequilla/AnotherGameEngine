#include <engine.hpp>

#include <iostream>
#include <stdexcept>
#include <format>
#include <chrono>

#include <utility/logging.hpp>

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

        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );

        m_sdlWindow = SDL_CreateWindow( 
            title.c_str(), 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            resolution.x, resolution.y, 
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );

        if(!m_sdlWindow)
        {
            throw std::runtime_error( std::format("ERROR: SDL Create Window => {}", SDL_GetError()) );
        }

        m_sdlGLContext = SDL_GL_CreateContext( m_sdlWindow ); 
        if( m_sdlGLContext == NULL )
        {
            throw std::runtime_error( std::format("ERROR: SDL GL Create Context => {}", SDL_GetError()) );
        }
    }

    Engine::~Engine()
    {
        SDL_GL_DeleteContext(m_sdlGLContext);
        m_sdlGLContext = NULL;

        SDL_DestroyWindow(m_sdlWindow);
        m_sdlWindow = nullptr;

        SDL_Quit();
    }

    bool Engine::run()
    {
        if(m_isRunning) return false; // already running
                                      
        uint32_t accumilatorFlushLog = 0;
        const uint32_t flushLogCount = 200;
        
        m_isRunning = true;

        std::chrono::time_point<std::chrono::high_resolution_clock> currentFrameTime, lastFrameTime;
        SDL_Event e;
        while( m_isRunning )
        {
            currentFrameTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            // TODO: Change to time based
            accumilatorFlushLog += 1;
            if(accumilatorFlushLog >= flushLogCount)
            {
                accumilatorFlushLog = 0;
                logger.status("Engine", std::format("Current frame time: {}.", deltaTime.count()));
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

            SDL_GL_SwapWindow( m_sdlWindow );
        }

        return true;
    }

}
