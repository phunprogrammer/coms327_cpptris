#include <audiomanager.h>

AudioManager::AudioManager() : terminateSound(false), terminateMusic(false) {
    SDL_Init(SDL_INIT_AUDIO);
    previous = std::chrono::steady_clock::now();
}

AudioManager::~AudioManager() {
    stopAllAudio();
    SDL_Quit();
}

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::playMusic(const std::string& filename, bool loop) {
    std::lock_guard<std::mutex> lock(musicMutex);

    if (musicThread.joinable())
        musicThread.join();

    musicThread = std::thread([this, filename, loop] { 
        playAudio(this->musicDevice, filename, loop, 0);
    });
}

void AudioManager::playSound(const std::string& filename, bool priority) {
    std::lock_guard<std::mutex> lock(soundMutex);

    if(soundThread.joinable()) {
        if(priority) {
            terminateSound = true; 
            soundThread.join();
        } else {
            return;
        }
    }

    soundThread = std::thread([this, filename] { 
        playAudio(this->soundDevice, filename, false, 1);
    });

    soundThread.detach();
}

void AudioManager::playAudio(SDL_AudioDeviceID device, const std::string& filename, bool loop, int type) {
    std::string filepath = AUDIO_PATH + filename;
    SDL_AudioSpec wavSpec;
    Uint8* wavBuffer;
    Uint32 wavLength;

    if (SDL_LoadWAV(filepath.c_str(), &wavSpec, &wavBuffer, &wavLength) == NULL)
        return;

    device = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    
    if (device == 0) {
        SDL_FreeWAV(wavBuffer);
        return;
    }

    SDL_QueueAudio(device, wavBuffer, wavLength);
    SDL_PauseAudioDevice(device, 0);

    if(type == 1) {
        while (SDL_GetQueuedAudioSize(device) > 0 && !terminateSound) {
            SDL_Delay(100);
        }
        terminateSound = false;
    }
    else {
        while (SDL_GetQueuedAudioSize(device) > 0 && !terminateMusic) {
            SDL_Delay(100);
        }
        terminateMusic = false;
    }
    
    SDL_ClearQueuedAudio(device);
    SDL_CloseAudioDevice(device);
    SDL_FreeWAV(wavBuffer);
}
void AudioManager::stopMusic() {
    std::lock_guard<std::mutex> lock(musicMutex);
    terminateMusic = true;
    if (musicThread.joinable()) {
        SDL_ClearQueuedAudio(musicDevice);
        musicThread.join();
        SDL_CloseAudioDevice(musicDevice);
    }
}

void AudioManager::stopSound() {
    std::lock_guard<std::mutex> lock(soundMutex);
    terminateSound = true;
    if (soundThread.joinable()) {
        SDL_ClearQueuedAudio(soundDevice);
        soundThread.join();
        SDL_CloseAudioDevice(soundDevice);
    }
}
void AudioManager::stopAllAudio() {
    stopMusic();
    stopSound();
}