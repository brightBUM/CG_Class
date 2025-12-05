#include "AudioSystem.h"
#include <iostream>
#include <fstream>

// =========================
// WAV LOADER (simple PCM 16-bit)
// =========================
ALuint AudioSystem::LoadWavFile(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open WAV: " << path << std::endl;
        return 0;
    }

    // --- Read WAV header ---
    char riff[4];
    file.read(riff, 4);              // "RIFF"
    file.ignore(4);                   // file size
    file.read(riff, 4);              // "WAVE"

    file.read(riff, 4);              // "fmt "
    file.ignore(4);                   // subchunk size

    short audioFormat, numChannels;
    int sampleRate;
    short bitsPerSample;

    file.read((char*)&audioFormat, 2);
    file.read((char*)&numChannels, 2);
    file.read((char*)&sampleRate, 4);
    file.ignore(6); // byte rate + block align
    file.read((char*)&bitsPerSample, 2);

    // data chunk
    char dataHeader[4];
    int dataSize;
    file.read(dataHeader, 4);        // "data"
    file.read((char*)&dataSize, 4);

    std::vector<char> pcm(dataSize);
    file.read(pcm.data(), dataSize);

    // Determine OpenAL format
    ALenum format = 0;
    if (bitsPerSample == 16)
        format = (numChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    else if (bitsPerSample == 8)
        format = (numChannels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;

    // Create buffer
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, pcm.data(), dataSize, sampleRate);

    std::cout << "Loaded WAV: " << path << std::endl;
    return buffer;
}

// =========================
// Initialize OpenAL
// =========================
bool AudioSystem::Init()
{
    device = alcOpenDevice(nullptr);
    if (!device) {
        std::cout << "Failed to open OpenAL device\n";
        return false;
    }

    context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);

    // Create audio sources (pool)
    alGenSources(MAX_SOURCES, sources);

    for (int i = 0; i < MAX_SOURCES; i++)
        alSourcef(sources[i], AL_GAIN, 1.0f);

    std::cout << "AudioSystem initialized.\n";
    return true;
}

// =========================
// Shutdown
// =========================
void AudioSystem::Shutdown()
{
    for (auto& pair : soundBuffers)
        alDeleteBuffers(1, &pair.second);

    alDeleteSources(MAX_SOURCES, sources);

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);

    std::cout << "AudioSystem shut down.\n";
}

// =========================
// Load sound + cache buffer
// =========================
unsigned int AudioSystem::LoadSound(const std::string& name, const std::string& filepath)
{
    if (soundBuffers.count(name))
        return soundBuffers[name];  // already loaded

    ALuint buffer = LoadWavFile(filepath);
    if (buffer)
        soundBuffers[name] = buffer;

    return buffer;
}

// =========================
// Find a free OpenAL source
// =========================
int AudioSystem::GetFreeSource()
{
    for (int i = 0; i < MAX_SOURCES; i++)
    {
        ALint state;
        alGetSourcei(sources[i], AL_SOURCE_STATE, &state);

        if (state != AL_PLAYING)
            return i;
    }

    return -1; // all busy
}

// =========================
// Play sound overlapping
// =========================
void AudioSystem::PlaySound(const std::string& name, float gain, float pitch)
{
    if (!soundBuffers.count(name)) {
        std::cout << "Sound not loaded: " << name << std::endl;
        return;
    }

    int src = GetFreeSource();
    if (src < 0) {
        std::cout << "No free audio sources! Increase pool.\n";
        return;
    }

    alSourceStop(sources[src]);
    alSourcei(sources[src], AL_BUFFER, soundBuffers[name]);
    alSourcef(sources[src], AL_GAIN, gain);
    alSourcef(sources[src], AL_PITCH, pitch);
    alSourcePlay(sources[src]);
}
