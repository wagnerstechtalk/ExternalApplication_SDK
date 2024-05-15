#include "sound.h"
#include <iostream>

// This is the constructor for the SoundManager class.
SoundManager::SoundManager(Core *core) 
{
    m_core = core;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 64) < 0) 
    {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}

// This is the destructor for the SoundManager class.
SoundManager::~SoundManager() 
{
    Mix_CloseAudio();
}

// The LoadSound function is used to load a sound from a file and store it in the sound resource
Mix_Chunk* SoundManager::LoadSound(const std::string id, const std::string filename) 
{
    Mix_Chunk* sound = Mix_LoadWAV_RW(SDL_RWFromFile(filename.c_str(), "rb"), 1);
    if (sound == nullptr) 
    {
        std::cerr << "Failed to load sound: " << filename << " Error: " << Mix_GetError() << std::endl;
        return nullptr;
    }

    m_soundResource[id] = sound;
    return sound;
}

// The GetSound function is used to get a sound from the sound resource
Mix_Chunk* SoundManager::GetSound(const std::string id)
{
    auto it = m_soundResource.find(id);
    if (it == m_soundResource.end())
    {
        std::cerr << "Sound with ID " << id << " not found!" << std::endl;
        return nullptr;
    }
    return it->second;
}

// The Play function is used to play a sound
int SoundManager::Play(Mix_Chunk *sound) 
{
    if(!sound)
    {
        std::cerr << "Sound is null!" << std::endl;
        return -1;
    }

    return Mix_PlayChannelTimed(-1, sound, 0, -1);
}

// The Stop function is used to stop a sound
bool SoundManager::IsPlaying(int channel)
{
    return Mix_Playing(channel);
}
