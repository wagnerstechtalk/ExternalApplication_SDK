#pragma once
#include "main.h"
#include "Snake.h"
#include "Food.h"
#include "gfx/image.h"
#include "gfx/font.h"
#include "gfx/sound.h"
#include "ColorManager.h"

class Core;

namespace std {
    template <>
    struct hash<SDL_Color> {
        size_t operator()(const SDL_Color& color) const {
            return (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;
        }
    };
}

struct SDL_ColorEqual {
    bool operator()(const SDL_Color& a, const SDL_Color& b) const {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
    }
};

enum kGameState{
    kGameState_MainMenu,
    kGameState_Ingame,
    kGameState_GameOver,
};

class GameLogic{
public:
    GameLogic(Core *core);

    void reset();
    void input(SDL_Scancode button);
    void input(SDL_GameControllerButton button);
    void logic(double deltaTime);
    void logic();
    void render();
    void generateNewFood();
    bool isCoordinateInVector(const SDL_Point& coordinate, const std::vector<SDL_Point>& pointVector);
    
private:
    double m_logicTime;
    float m_snakeSpeed;

    int m_grid_size;
    int m_grid_width;
    int m_grid_height;

    

    Snake m_snake;    
    std::vector<Food> m_foods;
    
    Uint32 m_lastFoodEatenTime;
    Uint32 m_time_to_eat;
    Uint32 m_elapsedMillisecondsEat;
    double m_scale;
    SDL_Point m_direction;
    SDL_Point m_prev_direction;
    int m_score;
    int m_highScore;
    float m_speed_level;
    bool m_foodEaten;
    std::unordered_map<std::string,std::unordered_map<SDL_Color, ImageManager::RenderTexture, std::hash<SDL_Color>, SDL_ColorEqual>> m_colorOverlayCache;	
    kGameState m_gameState;

    ColorManager m_colorManager;

    Core *m_core;
    void drawSnake(const std::vector<SDL_Point>& body, const std::vector<SDL_Color>& body_color);
    void drawFood(const SDL_Point& foodLocation, int index);
};
