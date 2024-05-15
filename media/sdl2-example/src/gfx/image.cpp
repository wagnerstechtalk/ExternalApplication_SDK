#include "image.h"
#include "core.h"

// The constructor for the ImageManager class
ImageManager::ImageManager(Core *core)
{
    m_core = core;
}

// The destructor for the ImageManager class
ImageManager::~ImageManager()
{
    FreeImages();
}

// The LoadImage function is used to load an image from a file and store it in the image resource
ImageManager::RenderTexture ImageManager::LoadImage(const std::string id, const std::string filename)
{
    if(m_imageResource.find(id) != m_imageResource.end())
    {
        return m_imageResource[id];
    }

    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (!surface)
    {
        printf("Failed to load image %s: %s\n", filename.c_str(), IMG_GetError());
        return RenderTexture();
    }

    SDL_Texture* imageTexture = nullptr;
    if (m_core->getRenderer())
    {
        // Create texture from surface
        imageTexture = SDL_CreateTextureFromSurface(m_core->getRenderer(), surface);
        if (!imageTexture)
        {
            printf("Failed to create texture: %s\n", SDL_GetError());
            return RenderTexture();
        }
    }

    auto ret = RenderTexture(surface, imageTexture);
    m_imageResource[id] = ret;
    return ret;
}

// The GetImage function is used to get an image from the image resource
ImageManager::RenderTexture ImageManager::GetImage(const std::string id)
{
    auto it = m_imageResource.find(id);
    if (it == m_imageResource.end())
    {
        return {};
    }
    return it->second;
}

// The DrawImage function is used to draw an image to the screen
void ImageManager::DrawImage(RenderTexture image, int x, int y, float scale, const SDL_Color color)
{
    if (image.m_texture)
    {
        SDL_Rect destRect = { x, y, static_cast<int>(image.m_surface->w * scale), static_cast<int>(image.m_surface->h * scale) };
        // Apply color overlay
        SDL_SetTextureColorMod(image.m_texture, color.r, color.g, color.b);
        SDL_RenderCopy(m_core->getRenderer(), image.m_texture, nullptr, &destRect);
    }
}

// The DrawImage function is used to draw an image to the screen
void ImageManager::DrawImage(RenderTexture image, SDL_Rect rect, const SDL_Color color)
{
    if (image.m_texture)
    {
        SDL_SetTextureColorMod(image.m_texture, color.r, color.g, color.b);
        SDL_RenderCopy(m_core->getRenderer(), image.m_texture, nullptr, &rect);
    }
}

// The FreeImage function is used to free an image
void ImageManager::FreeImage(RenderTexture texture)
{
    if (texture.m_surface)
    {
        SDL_FreeSurface(texture.m_surface);
    }
}

// The FreeImages function is used to free all images
void ImageManager::FreeImages()
{
    for (auto& it : m_imageResource)
    {
        FreeImage(it.second);
    }
}

