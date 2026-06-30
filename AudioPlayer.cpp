#include "AudioPlayer.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdexcept>
#include <format>

const unsigned int BUFFER_SIZE = 64;

AudioPlayer::AudioPlayer() {
	ma_result result = ma_context_init(NULL, 0, NULL, &this->context);
	if (result != MA_SUCCESS) {
		throw std::runtime_error(
			std::format("Failed to initialize playback context. Error code: {}", static_cast<int>(result)));
	}

	enumerateAvailablePlaybackDevices();
}

AudioPlayer::~AudioPlayer() {
	ma_context_uninit(&context);

	if (pAvailablePlaybackDevices) {
		delete[] pAvailablePlaybackDevices;
	}
}

void AudioPlayer::enumerateAvailablePlaybackDevices() {
	// Copying the enumerated devices because ma_context_get_devices can only get called once 
	// per context. This prevents dangling ptr
	ma_device_info* pDeviceBuffer = nullptr;
	ma_result result = ma_context_get_devices(&context, &pDeviceBuffer, &playbackDeviceCount, NULL, NULL);
	if (result != MA_SUCCESS) {
		throw std::runtime_error(
			std::format("Failed to enumerate available devices. Error code: {}", static_cast<int>(result)));
		ma_context_uninit(&context);
	}

	pAvailablePlaybackDevices = new ma_device_info[playbackDeviceCount];
	for (int i = 0; i < playbackDeviceCount; i++) {
		pAvailablePlaybackDevices[i] = pDeviceBuffer[i];
	}
}

ma_device_info AudioPlayer::getDeviceInfo(int deviceIndex) {
	return pAvailablePlaybackDevices[deviceIndex];
}

ma_uint32 AudioPlayer::getDeviceCount() const {
	return playbackDeviceCount;
}
int AudioPlayer::getSelectedDeviceIndex() const {
	return selectedDeviceIndex;
}
void AudioPlayer::updateSelectedDevice(int deviceIndex) {
	selectedDeviceIndex = deviceIndex;
}
