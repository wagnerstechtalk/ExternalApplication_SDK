#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <unordered_map>
class Core;

class SoundManager {
public:
    SoundManager(Core *core);
    ~SoundManager();

    Mix_Chunk* LoadSound(const std::string id, const std::string filename);
    Mix_Chunk* GetSound(const std::string id);

    int Play(Mix_Chunk *sound);
    bool IsPlaying(int channel);

private:
    std::unordered_map<std::string, Mix_Chunk*> m_soundResource;
    Core *m_core;
};
