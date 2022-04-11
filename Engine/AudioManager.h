#pragma once
#include <FMOD/fmod.hpp>
#include "common_includes.h"

class AudioManager {
public:
    int m_channels;

    AudioManager();
    AudioManager(int channels, float sound_volume, float music_volume);
    ~AudioManager();
    void init(int channels, float sound_volume, float music_volume);
    void add_sound(std::string name, std::string file_name, float volume);
    void add_music(std::string name, std::string file_name, float volume);
    void play_sound(std::string name);
    void play_music(std::string name);
    void stop_music(std::string name);
    void pause_music(std::string name);
    void update();

private:
    FMOD::System* system;
    std::map<std::string, FMOD::Sound*> m_sounds;
    std::map<std::string, FMOD::Sound*> m_music;
    FMOD_RESULT result;
    FMOD::Channel* m_channel = 0;
    void* extradriverdata = 0;
    FMOD::ChannelGroup* m_sound_channel_group;
    FMOD::ChannelGroup* m_music_channel_group;
};