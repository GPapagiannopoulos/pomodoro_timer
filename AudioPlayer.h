#pragma once 
#include "miniaudio.h"

class AudioPlayer {
private:
	ma_device device;
	ma_device_config config;
	ma_context context;
	ma_device_info* pAvailablePlaybackDevices = nullptr;
	ma_uint32 playbackDeviceCount = 0;
	int selectedDeviceIndex = -1;

	void enumerateAvailablePlaybackDevices();
public:
	AudioPlayer();
	~AudioPlayer();

	AudioPlayer(const AudioPlayer& player) = delete;
	AudioPlayer& operator=(const AudioPlayer& player) = delete;
	AudioPlayer(AudioPlayer&& player) = delete;
	AudioPlayer& operator=(AudioPlayer&& player) = delete;

	ma_device_info getDeviceInfo(int deviceIndex);
	ma_uint32 getDeviceCount() const;
	int getSelectedDeviceIndex() const;
	void updateSelectedDevice(int deviceIndex);
};