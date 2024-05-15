#include "GameLogic.h"
#include "gfx/core.h"

GameLogic::GameLogic(Core *core)
{
    m_core = core;

    m_speed_level = 0.0f;
    m_snakeSpeed = 1.0f / 15.0f;

    m_scale = core->getReferenceScale();

    m_grid_size = 50 * m_scale;
    m_grid_width = m_core->getWidth() / (m_grid_size * m_scale);
    m_grid_height = m_core->getHeight() / (m_grid_size * m_scale);

    m_direction = {0, 0};
    m_prev_direction = { 0, 0 };
    m_score = 0;
    m_highScore = 0;
    m_gameState = kGameState_MainMenu;
    
    m_snake.reset({m_grid_width / 2, m_grid_height / 2});
}

void GameLogic::reset()
{
    m_lastFoodEatenTime = SDL_GetTicks();
    m_time_to_eat = SDL_GetTicks();
    m_foodEaten = false;
    m_foods.clear();
    m_foods.push_back(Food(m_colorManager.randomColor()));

    m_foods[0].setColor(m_colorManager.differentRandomColor({ 128, 0, 128, 255 }));
    m_foods[0].spawn(m_grid_width, m_grid_height);

    m_snake.reset({ m_grid_width / 2, m_grid_height / 2 });
    m_score = 0;
    m_gameState = kGameState_Ingame;
    m_direction = { 1, 0 };
    m_speed_level = 0;
    m_snakeSpeed = 1.0f / 15.0f;
}

void GameLogic::input(SDL_Scancode button)
{
    switch (button)
    {
    case SDL_SCANCODE_UP:
        m_prev_direction = { 0, -1 };
        break;
    case SDL_SCANCODE_DOWN:
        m_prev_direction = { 0, 1 };
        break;
    case SDL_SCANCODE_LEFT:
        m_prev_direction = { -1, 0 };
        break;
    case SDL_SCANCODE_RIGHT:
        m_prev_direction = { 1, 0 };
        break;
    case SDL_SCANCODE_R:
        if (m_gameState != kGameState_Ingame)
        {
            reset();
        }
        break;
    }
}

void GameLogic::input(SDL_GameControllerButton button)
{
    switch (button)
    {
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        m_prev_direction = { 0, -1 };
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        m_prev_direction = { 0, 1 };
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        m_prev_direction = { -1, 0 };
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        m_prev_direction = { 1, 0 };
        break;        
    case SDL_CONTROLLER_BUTTON_START:
    case SDL_CONTROLLER_BUTTON_A:
        if (m_gameState != kGameState_Ingame)
        {
            reset();
        }
        break;
    }
}

void GameLogic::logic(double deltaTime)
{
    // Update the logic time act like a fixed time step
    m_logicTime += deltaTime;
    while (m_logicTime > m_snakeSpeed)
    {
        m_logicTime -= m_snakeSpeed;
        logic();
    }
}

void GameLogic::logic()
{
    if (m_gameState == kGameState_Ingame)
    {
        // Check if the new direction is opposite to the previous direction
        if ((m_direction.x + m_prev_direction.x != 0) && (m_direction.y + m_prev_direction.y != 0))
        {
            m_direction = m_prev_direction;
        }
        m_snake.move(m_direction, m_grid_width, m_grid_height);

        // Check for collisions
        if (m_snake.checkCollision())
        {
            m_core->getSoundManager()->Play(m_core->getSoundManager()->GetSound("collision"));
            if (m_score > m_highScore)
            {
                m_highScore = m_score;
            }
            m_gameState = kGameState_GameOver;
        }

        for (size_t i = 0; i < m_foods.size(); i++)
        {
            const SDL_Point& foodLocation = m_foods[i].getLocation();
            for (const auto& segment : m_snake.getBody())
            {
                if (segment.x == foodLocation.x && segment.y == foodLocation.y)
                {
                    static bool isRed = false;
                    if (ColorManager::Compare(m_foods[i].getColor(), { 128, 0, 128, 255 }))
                    {
                        if (m_score > m_highScore)
                        {
                            m_highScore = m_score;
                        }
                        m_gameState = kGameState_GameOver;
                        m_core->getSoundManager()->Play(m_core->getSoundManager()->GetSound("collision"));
                        break;
                    }
                    if (ColorManager::Compare(m_foods[i].getColor(), { 0, 255, 0, 255 }))
                    {
                       m_speed_level += 0.5f;
                       m_snakeSpeed = 1.0f / (15.0f + m_speed_level);
                    }
                    else if (ColorManager::Compare(m_foods[i].getColor(), { 255, 255, 0, 255 }))
                    {
                        m_snake.grow(m_foods[i].getColor());
                        m_score++;
                    }
                    else if (ColorManager::Compare(m_foods[i].getColor(), { 255, 0, 0, 255 }))
                    {
                        isRed = true;
                        if (m_snake.getBody().size() > 3)
                        {
                            m_snake.getBody().pop_back();
                            m_snake.getBodyColor().pop_back();
                            m_snake.getBodyColor().back() = { 255, 0, 0, 255 };
                            m_foods[i].setColor(m_colorManager.differentRandomColor({ 255, 0, 0, 255 }));
                        }
                    }
                    else if (ColorManager::Compare(m_foods[i].getColor(), { 0, 0, 255, 255 }))
                    {
                        if (m_score >= 1)
                        {
                            m_score -= 2;
                        }
                        else
                        {
                            m_score = -1;
                        }
                    }

                    if (!isRed)
                    {
                        m_snake.grow(m_foods[i].getColor());
                    }

                    m_foods[i].spawn(m_grid_width, m_grid_height);
                    m_score++;
                    m_foodEaten = true;
                    m_lastFoodEatenTime = SDL_GetTicks();
                    m_time_to_eat = SDL_GetTicks();
                    m_core->getSoundManager()->Play(m_core->getSoundManager()->GetSound("eat"));
                    isRed = false;
                    break;
                }
            }
        }


        // Generate a new food only if the snake has eaten one
        if (m_foodEaten)
        {
            generateNewFood();
        }

        if (m_gameState == kGameState_Ingame)
        {
            // Check if it's time to respawn the food
            Uint32 currentTime = SDL_GetTicks();
            Uint32 elapsedMilliseconds = currentTime - m_lastFoodEatenTime;
            if (elapsedMilliseconds >= 15000)
            {
                for (size_t i = 0; i < m_foods.size(); i++)
                {
                    m_foods[i].spawn(m_grid_width, m_grid_height);
                    m_lastFoodEatenTime = currentTime;
                }
            }

            m_elapsedMillisecondsEat = currentTime - m_time_to_eat;
            if (m_elapsedMillisecondsEat >= 30000)
            {
                m_gameState = kGameState_GameOver;
            }
        }
    }
}

bool GameLogic::isCoordinateInVector(const SDL_Point& coordinate, const std::vector<SDL_Point>& pointVector)
{
    for (const SDL_Point& point : pointVector)
    {
        if (point.x == coordinate.x && point.y == coordinate.y)
        {
            return true;
        }
    }
    return false;
}

void GameLogic::generateNewFood()
{
    SDL_Color targetColor = { 128, 0, 128, 255 };
    SDL_Color newFoodColor = m_colorManager.randomColor();

    m_foods.push_back(Food(newFoodColor));
    m_foods.back().spawn(m_grid_width, m_grid_height);

    if (ColorManager::Compare(newFoodColor, targetColor))
    {
        while (isCoordinateInVector(m_foods.back().getLocation(), m_snake.getBody()))
        {
            m_foods.back().spawn(m_grid_width, m_grid_height);
        } 
    }
    m_foodEaten = false;
}

void GameLogic::render()
{    
    switch (m_gameState)
    {
    case kGameState_MainMenu:
        m_core->getFontManager()->DrawFont(            
            m_core->getFontManager()->GetFont("NotoSans-Regular-Small"),
            m_core->getWidth() / 2.0f, // x-coordinate for center alignment
            m_core->getHeight() / 2.0f, // y-coordinate for center alignment
            { 
                "Red food will shrink your size.",
                "Green food will boost your speed, making movement more challenging.",
                "Blue food deducts 2 points from your score.",
                "Yellow food will make you grow longer and grant an extra point.",
                "Beware the purple food! It's toxic and will end your game if consumed.",
                " ",
                "Press 'A' or 'START' to embark on your journey.",
                "Navigate with the 'DPAD' controls.",
                "Hit 'HOME' to exit the game."
            },
            TextAlignment::CENTER, // Horizontal alignment
            TextAlignment::CENTER  // Vertical alignment
        );
        break;

    case kGameState_Ingame:
        m_core->getFontManager()->DrawFont(            
            m_core->getFontManager()->GetFont("NotoSans-Regular"),
            0, 
            0,
            { std::string("Score: ") + std::to_string(m_score), std::string("High Score: ") + std::to_string(m_highScore) },
            TextAlignment::LEFT, 
            TextAlignment::TOP 
        );
        m_core->getFontManager()->DrawFont(            
            m_core->getFontManager()->GetFont("NotoSans-Regular"),
            m_core->getWidth() - 400,
            0,
            { std::string("Time Left: ") + std::to_string(30 - m_elapsedMillisecondsEat / 1000) },
            TextAlignment::RIGHT,
            TextAlignment::TOP  
        );
        drawSnake(m_snake.getBody(), m_snake.getBodyColor());
        for (size_t i = 0; i < m_foods.size(); i++)
        {
            drawFood(m_foods[i].getLocation(), i);
        }
        break;

    case kGameState_GameOver:
        m_core->getFontManager()->DrawFont(            
            m_core->getFontManager()->GetFont("NotoSans-Regular"),
            0, 
            0, 
            { std::string("High Score: ") + std::to_string(m_highScore) },
            TextAlignment::LEFT, 
            TextAlignment::TOP  
        );
        m_core->getFontManager()->DrawFont(            
            m_core->getFontManager()->GetFont("NotoSans-Regular"),
            m_core->getWidth() / 2.0f, // x-coordinate for center alignment
            m_core->getHeight() / 2.0f, // y-coordinate for center alignment
            { 
                "Your journey has ended.",
                "The final score is " + std::to_string(m_score),
                " ",
                "Press 'A' or 'START' to embark on your journey.",
                "Navigate with the 'DPAD' controls.",
                "Hit 'HOME' to exit the game."
            },
            TextAlignment::CENTER, 
            TextAlignment::CENTER  
        );
        break;
    }
}

void GameLogic::drawSnake(const std::vector<SDL_Point>& body, const std::vector<SDL_Color>& body_color)
{
    auto res_snake = m_core->getImageManager()->GetImage("snake");

    for (size_t i = 0; i < body.size(); ++i)
    {
        SDL_Rect destRect = {
            static_cast<int>(body[i].x * m_grid_size * m_scale),
            static_cast<int>(body[i].y * m_grid_size * m_scale),
            static_cast<int>(m_grid_size * m_scale),
            static_cast<int>(m_grid_size * m_scale)
        };
        m_core->getImageManager()->DrawImage(res_snake, destRect, body_color[i]);
    }
}

void GameLogic::drawFood(const SDL_Point &foodLocation, int index)
{
    auto res_food = m_core->getImageManager()->GetImage("food");
    SDL_Rect destRect = {
        static_cast<int>(foodLocation.x * m_grid_size * m_scale),
        static_cast<int>(foodLocation.y * m_grid_size * m_scale),
        static_cast<int>(m_grid_size * m_scale),
        static_cast<int>(m_grid_size * m_scale)
    };

    m_core->getImageManager()->DrawImage(res_food, destRect, m_foods[index].getColor());
}
