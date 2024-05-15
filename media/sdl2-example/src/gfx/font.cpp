#include "font.h"
#include "core.h"

// The constructor for the FontManager class
FontManager::FontManager(Core *core) 
{
    m_core = core;
}

// The destructor for the FontManager class
FontManager::~FontManager()
{
    FreeFonts();
}

// The LoadFont function is used to load a font from a file with specified size and store it in the font resource
TTF_Font* FontManager::LoadFont(const std::string id, const std::string filename, const int size)
{
    if(m_fontResource.find(id) != m_fontResource.end())
    {
        return m_fontResource[id];
    }

    TTF_Font* font = TTF_OpenFont(filename.c_str(), size);
    if (!font)
    {
        printf("Failed to load font %s: %s\n", filename.c_str(), TTF_GetError());
        return NULL;
    }

    m_fontResource[id] = font;
    return font;
}

// The GetFont function is used to get a font from the font resource
TTF_Font* FontManager::GetFont(const std::string id)
{
    auto it = m_fontResource.find(id);
    if (it == m_fontResource.end())
    {
        return nullptr;
    }
    return it->second;
}

// The GetMaxTextWidth function is used to get the maximum width of a text
int FontManager::GetMaxTextWidth(TTF_Font* font, const std::vector<std::string>& messages)
{
    int maxWidth = 0;
    for (const auto& message : messages)
    {
        if (!message.empty())
        {
            int textWidth, textHeight;
            TTF_SizeUTF8(font, message.c_str(), &textWidth, &textHeight);
            maxWidth = std::max(maxWidth, textWidth);
        }
    }
    return maxWidth;
}

// The GetTotalTextHeight function is used to get the total height of a text
int FontManager::GetTotalTextHeight(TTF_Font* font, const std::vector<std::string>& messages)
{
    int totalHeight = 0;
    for (const auto& message : messages)
    {
        if (!message.empty())
        {
            int textWidth, textHeight;
            TTF_SizeUTF8(font, message.c_str(), &textWidth, &textHeight);
            totalHeight += textHeight;
        }
    }
    return totalHeight;
}

// The DrawFont function is used to draw a font to the screen
void FontManager::DrawFont(TTF_Font* font, float x, float y, std::vector<std::string> messages, TextAlignment alignmentHorizontal, TextAlignment alignmentVertical)
{
    if (!font)
    {
        return;
    }

    int totalTextHeight = GetTotalTextHeight(font, messages);
    int startY = y;

    if (alignmentVertical == TextAlignment::CENTER)
    {
        startY -= totalTextHeight / 2;
    }
    else if (alignmentVertical == TextAlignment::BOTTOM)
    {
        startY -= totalTextHeight;
    }

    for (const auto& message : messages)
    {
        if (message.empty())
        {
            continue;
        }

        int textWidth, textHeight;
        TTF_SizeUTF8(font, message.c_str(), &textWidth, &textHeight);

        int adjustedX = x;

        if (alignmentHorizontal == TextAlignment::CENTER)
        {
            adjustedX -= textWidth / 2;
        }
        else if (alignmentHorizontal == TextAlignment::RIGHT)
        {
            adjustedX -= textWidth;
        }

        SDL_Color fg = { 255, 255, 255, 255 };
        SDL_Surface* surface_tmp = TTF_RenderUTF8_Blended(font, message.c_str(), fg);

        if (surface_tmp)
        {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(m_core->getRenderer(), surface_tmp);
            if (texture)
            {
                SDL_Rect srcRect = { 0, 0, surface_tmp->w, surface_tmp->h };
                SDL_Rect destRect = { adjustedX, startY, surface_tmp->w, surface_tmp->h };
                SDL_RenderCopy(m_core->getRenderer(), texture, &srcRect, &destRect);

                SDL_DestroyTexture(texture);
            }

            SDL_FreeSurface(surface_tmp);
        }

        startY += textHeight;
    }
}

// The FreeFont function is used to free a font
void FontManager::FreeFont(TTF_Font* font)
{
    if (font)
    {
        TTF_CloseFont(font);
        font = NULL;
    }
}

// The FreeFonts function is used to free all fonts
void FontManager::FreeFonts()
{
    for (auto& it : m_fontResource)
    {
        FreeFont(it.second);
    }
}
