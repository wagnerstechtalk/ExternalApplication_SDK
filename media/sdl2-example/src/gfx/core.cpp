#include "core.h"

#include "font.h"
#include "image.h"
#include "sound.h"

// The constructor for the Core class
Core::Core()
{
    m_windowWidth = 1280;
    m_windowHeight = 720;
    m_renderer = nullptr;
    m_texture = nullptr;

    m_exit = false;

    m_lastTick = 0;
    m_currentTick = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
    }

    m_imageManager = new ImageManager(this);
    m_fontManager = new FontManager(this);
    m_soundManager = new SoundManager(this);
}

// The destructor for the Core class
Core::~Core()
{
    if (m_texture)
    {
        SDL_DestroyTexture(m_texture);
    }

    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
    }
                
    for (auto controller : m_controllers)
    {
        if (controller)
        {
            SDL_GameControllerClose(controller);
        }            
    }

    m_controllers.clear();

    TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// The init function is init the SDL2 library and create the window and renderer.
int Core::initGFX()
{

    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
    {
        std::cerr << "SDL_GetCurrentDisplayMode failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_FULLSCREEN);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GetWindowSize(window, &m_windowWidth, &m_windowHeight);
    std::cout << "Window resolution: " << m_windowWidth << " x " << m_windowHeight << std::endl;

    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    m_texture = nullptr;

    SDL_ShowCursor(SDL_DISABLE);

    return 0;
}

// The rotateScreen function is used to rotate the screen. 
// It create a virtual screen via a texture with the new orientation.
// Then the game is rendered to the virtual screen.
// Then the virtual screen is rendered to the actual screen to achieve the correct orientation for the game.
void Core::rotateScreen()
{
    std::swap(m_windowWidth, m_windowHeight);

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_windowWidth, m_windowHeight);
    if (!m_texture)
    {
        std::cerr << "SDL_CreateTexture failed: " << SDL_GetError() << std::endl;
        return;
    }
}

// The initInput function is used to initialize the SDL2 game controller. 
// Every game controller has a unique layout of buttons and axes. That way we need to load the layout from a file. This file is generated by the system.
int Core::initInput()
{
    SDL_GameControllerAddMappingsFromFile("/userdata/customer_controller_db_3rd.txt");

    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        if (SDL_IsGameController(i))
        {
            m_controllers.push_back(SDL_GameControllerOpen(i));
            std::cout << i << " slot: '" << SDL_GameControllerName(m_controllers.back()) << "' name" << std::endl;
        }
    }

    return 0;
}

// The addInput function is used to add a new game controller to the list of connected controllers.
void Core::addInput(int idx)
{
    if (SDL_IsGameController(idx))
    {
        SDL_GameController *controller = SDL_GameControllerOpen(idx);

        if (controller != NULL)
        {
            m_controllers[idx] = controller;
            printf("Controller %d connected\n", idx);
        }
    }
}

// The removeInput function is used to remove a game controller from the list of connected controllers.
void Core::removeInput(int idx)
{
    if (m_controllers[idx] != NULL)
    {
        SDL_GameControllerClose(m_controllers[idx]);
        m_controllers[idx] = NULL;
        printf("Controller %d disconnected\n", idx);
    }
}

// The clearScreen function is used to clear the screen with a specific color.
void Core::clearScreen(float r, float g, float b, float a)
{
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    SDL_RenderClear(m_renderer);
}

// The beginScreen function is used to start rendering the game to the screen. 
// This handles the virtual screen if the screen is rotated.
void Core::beginScreen()
{
    //Check if there is a virtual screen
    if(m_texture)
    {
        // Set the virtual screen as the render target
        SDL_SetRenderTarget(m_renderer, m_texture);
    }

    clearScreen(127, 127, 127, 255);
}

// The endScreen function is used to end rendering the game to the screen.
void Core::endScreen()
{
    //Check if there is a virtual screen
    if(m_texture)
    {
	    // Reset render target to the window
	    SDL_SetRenderTarget(m_renderer, NULL);

        // Render the virtual screen onto the screen with rotation
        SDL_Rect destRect = {(m_windowHeight-m_windowWidth)/2, (m_windowWidth-m_windowHeight)/2, m_windowWidth, m_windowHeight};		
        SDL_RenderCopyEx(m_renderer, m_texture, NULL, &destRect, 90, nullptr, SDL_FLIP_NONE);
    }

    SDL_RenderPresent(m_renderer);
}

// The handleEvents function is used to handle the SDL2 events.
void Core::handleEvents()
{	
    m_currentTick = SDL_GetTicks();
    if(m_lastTick == 0)
    {
        // Fix the first frame delta time
        m_deltaTime = 1/60.0f;
    }
    else
    {
        m_deltaTime = (m_currentTick - m_lastTick) / 1000.0f; // deltaTime in seconds
    }    
    m_lastTick = m_currentTick;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: // The user wants to exit the game
            m_exit = true;
            break;

        case SDL_CONTROLLERDEVICEADDED: // A new game controller is connected
            addInput(event.cdevice.which);
            break;

        case SDL_CONTROLLERDEVICEREMOVED: // A game controller is disconnected
            removeInput(event.cdevice.which);
            break;

        case SDL_CONTROLLERBUTTONDOWN: // A button on the game controller is pressed
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE)
            {
                m_exit = true;
            }            
            break;

        case SDL_KEYDOWN: // A key on the keyboard is pressed
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                m_exit = true;
            }            
            break;
        }

        m_events.push(event);
    }
}

// The isRunning function is used to check if the game is running.
bool Core::isRunning()
{
    return !m_exit;
}