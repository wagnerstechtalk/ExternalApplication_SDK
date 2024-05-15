#include "main.h"

#include "Snake.h"
#include "Food.h"
#include "GameLogic.h"

#include "gfx/core.h"
#include "gfx/font.h"
#include "gfx/image.h"

void loadAssest(Core &core)
{
	std::cout << "Loading assests" << std::endl;

	core.getFontManager()->LoadFont("NotoSans-Regular", "res/NotoSans-Regular.ttf", 48 * core.getReferenceScale());
	core.getFontManager()->LoadFont("NotoSans-Regular-Small", "res/NotoSans-Regular.ttf", 28 * core.getReferenceScale());
	
	core.getImageManager()->LoadImage("snake", "res/snake.png");
	core.getImageManager()->LoadImage("food", "res/food.png");

	core.getSoundManager()->LoadSound("eat", "res/eat.wav");
	core.getSoundManager()->LoadSound("collision", "res/collision.wav");
}

int main(int argc, char **argv)
{
	// Seed the random number generator
	srand(static_cast<unsigned int>(time(nullptr)));

	Core core;
	
	if (core.initGFX() < 0)
	{		
		std::cout << "The graphics could not be initialized" << std::endl;		
		return 0;
	}

	if (core.initInput() < 0)
	{
		std::cout << "The input could not be initialized" << std::endl;
		return 0;
	}

	// Rotate the screen by 90 degrees to support portrait orientation
	core.rotateScreen();

	// Load assests
	loadAssest(core);
	
	// Create the game logic
	GameLogic snakeGame(&core);

	while (core.isRunning())
	{
		core.handleEvents();

		while(core.hasEvent())
		{
			auto event = core.popEvent();
			switch(event.type)
			{
				case SDL_CONTROLLERBUTTONDOWN:
					snakeGame.input((SDL_GameControllerButton)event.cbutton.button);
					break;
				case SDL_KEYDOWN:
					snakeGame.input((SDL_Scancode)event.key.keysym.scancode);
            		break;
			}
		}
		
		// Update the game logic
		snakeGame.logic(core.getDeltaTime());

		// Render the game
		core.beginScreen();
		snakeGame.render();
		core.endScreen();
	}

	return 0;
}
