#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

// Window dimensions
const int WINDOW_WIDTH = 3840;
const int WINDOW_HEIGHT = 2160;

// Ball properties
const int BALL_SIZE = 20;
const int BALL_SPEED = 15;
// Controllers
std::vector<SDL_GameController *> controllers(SDL_CONTROLLER_BUTTON_MAX);

void loadControllerDB()
{
	printf("loadControllerDB\n");
	SDL_GameControllerAddMappingsFromFile("/userdata/customer_controller_db_3rd.txt");

	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (SDL_IsGameController(i))
		{
			controllers[i] = SDL_GameControllerOpen(i);
			printf("%d slot: '%s' name\n", i, SDL_GameControllerName(controllers[i]));
		}
	}
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    loadControllerDB();

    // Create window
    SDL_Window* window = SDL_CreateWindow("Bouncing Ball", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize ball position and velocity
    int ballX = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
    int ballY = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
    int ballVelX = BALL_SPEED;
    int ballVelY = BALL_SPEED;

    Uint32 lastTick = SDL_GetTicks();

    // Main loop flag
    bool bExit = false;
    
    // Main loop
    while (!bExit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    bExit = true;
                    break;
                case SDL_KEYDOWN:
                    bExit = true;
                    break;
                case SDL_JOYBUTTONDOWN:
                    bExit = true;
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    if ( SDL_NumJoysticks() > 0 ) {
                        switch (e.cbutton.button) {
                        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X:
                            bExit = true;
                        }
                    }
                    break;
                case SDL_CONTROLLERDEVICEADDED:
                    if (SDL_IsGameController(e.cdevice.which))
                    {
                        SDL_GameController *controller = SDL_GameControllerOpen(e.cdevice.which);

                        if (controller != NULL)
                        {
                            controllers[e.cdevice.which] = controller;
                        }
                    }
                    break;

                case SDL_CONTROLLERDEVICEREMOVED:
                    if (controllers[e.cdevice.which] != NULL)
                    {
                        SDL_GameControllerClose(controllers[e.cdevice.which]);
                        controllers[e.cdevice.which] = NULL;
                    }
                    break;
            }
        }

        // Update ball position
        ballX += ballVelX;
        ballY += ballVelY;

        // Check for collisions with window boundaries
        if (ballX < 0 || ballX + BALL_SIZE > WINDOW_WIDTH) {
            ballVelX = -ballVelX;
        }
        if (ballY < 0 || ballY + BALL_SIZE > WINDOW_HEIGHT) {
            ballVelY = -ballVelY;
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the ball
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect ballRect = {ballX, ballY, BALL_SIZE, BALL_SIZE};
        SDL_RenderFillRect(renderer, &ballRect);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Delay to control frame rate
        SDL_Delay(16); // ~60 FPS
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    for (auto &it : controllers)
	{
		if (it != nullptr)
		{
			SDL_GameControllerClose(it);
			it = nullptr;
		}
	}

    SDL_Quit();
    
    return 0;
}
