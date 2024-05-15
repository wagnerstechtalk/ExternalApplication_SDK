#include "Snake.h"
#include <vector>
#include <random>

// The constructor for the Snake class
Snake::Snake()
{
}

// The reset function is used to reset the snake's body to the starting position
void Snake::reset(const SDL_Point location)
{
    m_body.clear();
    m_body.push_back(location);
    m_body.push_back({location.x - 1, location.y});
    m_body.push_back({location.x - 2, location.y});

    m_body_color.clear();
    m_body_color.push_back({ 0, 255, 0, 255 });
    m_body_color.push_back({ 0, 0, 255, 255 });
    m_body_color.push_back({ 255, 0, 0, 255 });
}

// The move function is used to move the snake's body
void Snake::move(const SDL_Point moveDir, int grid_width, int grid_height)
{
    // Move the snake's body
    for (int i = m_body.size() - 1; i > 0; i--)
    {
        m_body[i].x = m_body[i - 1].x;
        m_body[i].y = m_body[i - 1].y;
    }
    // Move the head with wrapping around behavior
    m_body[0].x = (m_body[0].x + moveDir.x + grid_width) % grid_width;
    m_body[0].y = (m_body[0].y + moveDir.y + grid_height) % grid_height;
}

// The grow function is used to grow the snake's body
void Snake::grow(const SDL_Color color)
{
    // Add a new segment to the snake's m_body
    if (!m_body.empty())
    {
        m_body.push_back(m_body.back());
        m_body_color.push_back(color);
    }
}

// The checkCollision function is used to check if the snake's head collides with its body or the screen edges
bool Snake::checkCollision()
{
    int headX = m_body[0].x;
    int headY = m_body[0].y;

    for (size_t i = 1; i < m_body.size(); i++)
    {
        if (m_body[i].x == headX && m_body[i].y == headY)
        {
            return true; // Collision with its m_body
        }
    }
    return false;
}

// The getm_body function is used to get the snake's body
std::vector<SDL_Point> &Snake::getBody()
{
    return m_body;
}

std::vector<SDL_Color> &Snake::getBodyColor()
{
    return m_body_color;
}