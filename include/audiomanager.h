#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <atomic>

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#define AUDIO_PATH "./data/audio/"

class AudioManager {
public:
    static AudioManager& getInstance();

private:
    AudioManager();
    ~AudioManager();

    SDL_AudioDeviceID musicDevice;
    SDL_AudioDeviceID soundDevice;

    std::thread musicThread;
    std::thread soundThread;
    std::mutex musicMutex;
    std::mutex soundMutex;

    std::chrono::time_point<std::chrono::steady_clock> previous;
    bool terminateSound;
    bool terminateMusic;

public:
    AudioManager(AudioManager const&) = delete;
    void operator=(AudioManager const&) = delete;

    void playMusic(const std::string& filename, bool loop = false);
    void playSound(const std::string& filename, bool priority = true);
    void stopMusic();
    void stopSound();
    void stopAllAudio();
    void playAudio(SDL_AudioDeviceID device, const std::string& filename, bool loop, int type);
};

#endif