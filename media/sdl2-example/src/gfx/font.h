#pragma once
#include "../main.h"
#include <unordered_map>
class Core;

enum class TextAlignment
{
    LEFT,
    CENTER,
    RIGHT,
    BOTTOM,
    TOP
};

class FontManager
{
public:
    FontManager(Core *core);
    ~FontManager();

    TTF_Font* LoadFont(const std::string id, const std::string filename, const int size);
    TTF_Font* GetFont(const std::string id);

    void DrawFont(TTF_Font* font, float x, float y, std::vector<std::string> messages, TextAlignment alignmentHorizontal, TextAlignment alignmentVertical);
    int GetMaxTextWidth(TTF_Font* font, const std::vector<std::string>& messages);
    int GetTotalTextHeight(TTF_Font* font, const std::vector<std::string>& messages);
    void FreeFont(TTF_Font* font);
    void FreeFonts();

private:
    std::unordered_map<std::string, TTF_Font*> m_fontResource;
    Core *m_core;
};