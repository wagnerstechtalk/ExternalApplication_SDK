#pragma once
#include "main.h"

class Snake
{
private:
    std::vector<SDL_Point> m_body;
    std::vector<SDL_Color> m_body_color;

public:
    Snake();

    void reset(const SDL_Point location);
    void move(const SDL_Point moveDir, int grid_width, int grid_height);
    void grow(const SDL_Color color);
    bool checkCollision();
    std::vector<SDL_Point> &getBody();
    std::vector<SDL_Color> &getBodyColor();
};
