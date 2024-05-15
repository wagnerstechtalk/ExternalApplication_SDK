#pragma once
#include "../main.h"
#include <unordered_map>
class Core;

class ImageManager
{
public:
    struct RenderTexture
    {
        SDL_Surface *m_surface;
        SDL_Texture *m_texture;

        RenderTexture()
        {
            m_surface = NULL;
            m_texture = NULL;
        }

        RenderTexture(SDL_Surface *surface, SDL_Texture *texture)
        {
            m_surface = surface;
            m_texture = texture;
        }

        bool valid() const
        {
            return m_surface;
        }
    };

    ImageManager(Core *core);
    ~ImageManager();

    RenderTexture LoadImage(const std::string id, const std::string filename);
    RenderTexture GetImage(const std::string id);

    void DrawImage(RenderTexture image, int x, int y, float scale, const SDL_Color color = {255, 0, 0, 255});
    void DrawImage(RenderTexture image, SDL_Rect rect, const SDL_Color color = {255, 0, 0, 255});
    
    void FreeImage(RenderTexture texture);
    void FreeImages();
    
private:
    std::unordered_map<std::string, RenderTexture> m_imageResource;
    Core *m_core;
};
