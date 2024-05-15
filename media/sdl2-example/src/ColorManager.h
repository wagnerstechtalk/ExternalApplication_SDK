#pragma once
#include <random>
#include <vector>
#include <SDL2/SDL.h>

class ColorManager{
private:
    std::vector<SDL_Color> m_colors = {
        {255, 0, 0, 255},   // Red
        {0, 255, 0, 255},   // Green
        {0, 0, 255, 255},    // Blue
        {255, 255, 0, 255},    // Yellow
        {128, 0, 128, 255}   // Purple
    };
    std::random_device m_rd;
    std::mt19937 m_gen;
    std::uniform_int_distribution<> m_dist;

public:

    ColorManager(): m_gen(m_rd()), m_dist(0, m_colors.size() - 1)
    {

    }

    static bool Compare(const SDL_Color& a, const SDL_Color& b)
    {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
    }

    SDL_Color differentRandomColor(const SDL_Color& comparedColor)
    {
        SDL_Color foodColor;
        do
        {
            foodColor = randomColor();
        } while (Compare(foodColor, comparedColor));
        return foodColor;
    }

    SDL_Color randomColor()
    {
        int randomIndex = m_dist(m_gen);
        SDL_Color randomColor = m_colors[randomIndex];

        return randomColor;
    }
};