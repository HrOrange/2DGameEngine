#include "AudioManager.h"
#include "global_variables.h"

AudioManager::AudioManager() {

}
AudioManager::AudioManager(int channels, float sound_volume, float music_volume)
	:m_channels(channels) {
	result = FMOD::System_Create(&system);
	result = system->init(10, FMOD_INIT_NORMAL, extradriverdata); //32 er måske for meget eller for lidt, vi får se. (formentligt for meget, jeg tror i hvert fald ikke at 10 lyde skal spilles på samme tid)
	
	system->createChannelGroup("Sound", &m_sound_channel_group);
	system->createChannelGroup("Music", &m_music_channel_group);
	m_music_channel_group->setVolume(music_volume);
}
void AudioManager::init(int channels, float sound_volume, float music_volume) {
	m_channels = channels;
	result = FMOD::System_Create(&system);
	result = system->init(10, FMOD_INIT_NORMAL, extradriverdata); //32 er måske for meget eller for lidt, vi får se. (formentligt for meget, jeg tror i hvert fald ikke at 10 lyde skal spilles på samme tid)

	system->createChannelGroup("Sound", &m_sound_channel_group);
	system->createChannelGroup("Music", &m_music_channel_group);
	m_music_channel_group->setVolume(music_volume);
}
AudioManager::~AudioManager() {
	for (auto const& [key, val] : m_sounds) {
		val->release();
	}
}
void AudioManager::add_sound(std::string name, std::string file_name, float volume) {
	FMOD::Sound* sound;
	result = system->createSound((PATH + "\\" + file_name).c_str(), FMOD_DEFAULT, 0, &sound);
	result = sound->setMode(FMOD_LOOP_OFF);

	m_sounds.insert({name, sound});
}
void AudioManager::add_music(std::string name, std::string file_name, float volume) {
	FMOD::Sound* music;
	result = system->createSound((PATH + "\\" + file_name).c_str(), FMOD_LOOP_NORMAL, 0, &music);

	m_music.insert({ name, music });
}
void AudioManager::play_sound(std::string name) {
	system->playSound(m_sounds[name], m_sound_channel_group, false, 0); //det 0 skal muske ændres
}
void AudioManager::play_music(std::string name) {
	system->playSound(m_music[name], m_music_channel_group, false, 0); //det 0 skal muske ændres
}
void AudioManager::stop_music(std::string name) {

}
void AudioManager::pause_music(std::string name) {

}
void AudioManager::update() {
	result = system->update();
	//println(result);
}