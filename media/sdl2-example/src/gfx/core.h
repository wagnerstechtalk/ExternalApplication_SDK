#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <vector>
#include <iostream>
#include <queue>

class ImageManager;
class FontManager;
class SoundManager;

class Core {
private:
    int m_windowWidth, m_windowHeight;
    SDL_Renderer *m_renderer;
    std::vector<SDL_GameController *> m_controllers;

    bool m_exit;

    uint32_t m_lastTick;	
    uint32_t m_currentTick;
    double m_deltaTime;

    SDL_Texture* m_texture;

    ImageManager *m_imageManager;
	FontManager *m_fontManager;
    SoundManager *m_soundManager;

    std::queue<SDL_Event> m_events;

    void clearScreen(float r, float g, float b, float a);

    void addInput(int idx);
    void removeInput(int idx);
public:
    Core();
    ~Core();

    int initGFX();
    int initInput();

    void rotateScreen();

    bool hasEvent()
    {
        return !m_events.empty();
    }

    SDL_Event popEvent()
    {
        SDL_Event event = m_events.front();
        m_events.pop();
        return event;
    }

    void beginScreen();
    void endScreen();

    void handleEvents();
    bool isRunning();

    double getDeltaTime()
    {
        return m_deltaTime;
    }

    SDL_Renderer *getRenderer()
    {
        return m_renderer;
    }

    ImageManager *getImageManager()
    {
        return m_imageManager;
    }

    FontManager *getFontManager()
    {
        return m_fontManager;
    }

    SoundManager *getSoundManager()
    {
        return m_soundManager;
    }

    // Make sure the font is scaled correctly for the screen size
    double getReferenceScale()
    {
        const double REF_SCREEN_HEIGHT = 1080.0;
        
        if(m_texture)
        {
            return getWidth() / REF_SCREEN_HEIGHT;
        }
            
        return getHeight() / REF_SCREEN_HEIGHT;
    }

    int getWidth()
    {
        return m_windowWidth;
    }

    int getHeight()
    {
        return m_windowHeight;
    }

};