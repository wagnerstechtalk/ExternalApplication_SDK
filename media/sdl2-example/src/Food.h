#pragma once
#include "main.h"

class Food
{
public:
    Food(SDL_Color color);

    void setColor(SDL_Color color);
    void spawn(int grid_width, int grid_height);

    SDL_Point getLocation();
    SDL_Color getColor();

    
private:
    SDL_Point m_location;
    SDL_Color m_color;    
};
