#pragma once 
#include "miniaudio.h"
#include <string>
#include <mutex>
#include <condition_variable>

class AudioPlayer {
private:
	bool deviceInitialized = false;
	bool decoderInitialized = false;
	bool loopSound = false;
	bool alertPlayed = false;
	std::mutex mutex; 
	std::condition_variable c_var;

	ma_device device;
	ma_decoder decoder;
	ma_context context;
	ma_device_info* pAvailablePlaybackDevices = nullptr;
	ma_uint32 playbackDeviceCount = 0;
	int selectedDeviceIndex = -1;

	void enumerateAvailablePlaybackDevices();
	void initializeDecoder(std::string file_path);
	void initializePlaybackDevice();

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
	void updateLoopSound();
	bool getAlertStatus() const;
	void updateAlertStatus();
	void updateSelectedDevice(int deviceIndex);

	void uninitDevice();
	void uninitDecoder();
	void playAlert();
	void playAmbientNoise();
	void stopAudioDevice();
};