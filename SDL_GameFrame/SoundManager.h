#ifndef __SoundManager__
#define __SoundManager__

#include <iostream>
#include <map>
#include <string>
#include "SDL_mixer.h"

enum sound_type
{
    SOUND_MUSIC = 0,
    SOUND_SFX = 1
};

class SoundManager
{
public:
    
    static SoundManager* Singleton()
    {
        if(s_pSingleton == 0)
        {
            s_pSingleton = new SoundManager();
            return s_pSingleton;
        }
        return s_pSingleton;
    }
    
    bool load(std::string fileName, std::string id, sound_type type);
    
    void playSound(std::string id, int loop);
    void playMusic(std::string id, int loop);
    
private:
    
    static SoundManager* s_pSingleton;
    
    std::map<std::string, Mix_Chunk*> m_sfxs;
    std::map<std::string, Mix_Music*> m_music;
    
    SoundManager();
    ~SoundManager();
    
    SoundManager(const SoundManager&);
	SoundManager& operator=(const SoundManager&);
};

typedef SoundManager SoundManager;

#endif // __SoundManager__