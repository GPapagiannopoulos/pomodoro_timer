#pragma once 
#include "miniaudio.h"
#include <string>

class AudioPlayer {
private:
	bool deviceInitialized = false;
	bool decoderInitialized = false;
	bool loopSound = false;

	ma_device device;
	ma_decoder decoder;
	ma_context context;
	ma_device_info* pAvailablePlaybackDevices = nullptr;
	ma_uint32 playbackDeviceCount = 0;
	int selectedDeviceIndex = -1;

	void enumerateAvailablePlaybackDevices();
	void initializeDecoder(std::string file_path);
	void initializePlaybackDevice();
	void playAlertSound();

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
	ma_decoder* getDecoder();
	bool getLoopSound() const;
	void updateSelectedDevice(int deviceIndex);

	void startAudioDevice();
	void stopAudioDevice();
};