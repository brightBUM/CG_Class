#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <vector>
#include <map>

class AudioSystem
{
public:
    bool Init();
    void Shutdown();

    // Loads a sound file into a buffer and keeps it cached.
    // Returns buffer ID.
    unsigned int LoadSound(const std::string& name, const std::string& filepath);

    // Plays a sound using source pool (allows overlap)
    void PlaySound(const std::string& name, float gain = 1.0f, float pitch = 1.0f);

private:
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;

    std::map<std::string, ALuint> soundBuffers;

    static const int MAX_SOURCES = 32;
    ALuint sources[MAX_SOURCES];

    int GetFreeSource();
    ALuint LoadWavFile(const std::string& path);
};
