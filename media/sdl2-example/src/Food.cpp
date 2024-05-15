#include "Food.h"

// The constructor for the Food class
Food::Food(SDL_Color color)
{
    m_color = color;
}

// The spawn function is used to generate a random position for the food
void Food::spawn(int grid_width, int grid_height)
{
    // Generate a random position for the food
    int x = rand() % grid_width;
    int y = rand() % grid_height;
    m_location = {x, y};
}

// The getLocation function is used to get the location of the food
SDL_Point Food::getLocation()
{
    return m_location;
}

// The setColor function is used to set the color of the food
void Food::setColor(SDL_Color color)
{
    m_color = color;
}

// The getColor function is used to get the location of the food
SDL_Color Food::getColor()
{
    return m_color;
}