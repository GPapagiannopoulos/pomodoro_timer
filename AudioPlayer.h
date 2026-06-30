#pragma once 
#include "miniaudio.h"

class AudioPlayer {
private:
	ma_device device;
	ma_device_config config;
	ma_context context;
	ma_device_info* pAvailablePlaybackDevices;

public:
	AudioPlayer();
	~AudioPlayer();

	AudioPlayer(const AudioPlayer& player) = delete;
	AudioPlayer& operator=(const AudioPlayer& player) = delete;
	AudioPlayer(AudioPlayer&& player) = delete;
	AudioPlayer& operator=(AudioPlayer&& player) = delete;
};