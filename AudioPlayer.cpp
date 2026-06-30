#include "AudioPlayer.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdexcept>
#include <format>
#include <filesystem>

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
	if (deviceInitialized) {
		ma_device_uninit(&device);
	}
	if (decoderInitialized) {
		ma_decoder_uninit(&decoder);
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

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	ma_decoder* decoder = static_cast<ma_decoder*>(pDevice->pUserData);

	ma_uint64 frames_read = 0;
	ma_decoder_read_pcm_frames(decoder, pOutput, frameCount, &frames_read);

	if (frames_read < frameCount) {
		ma_decoder_seek_to_pcm_frame(decoder, 0);
	}
}

void AudioPlayer::startAudioDevice() {
	ma_result decoder_init_result = ma_decoder_init_file("C:/Users/georg/source/repos/pomodoro_timer/sounds/brown_noise/smooth_brown_noise.mp3", NULL, &decoder);
	if (decoder_init_result != MA_SUCCESS) {
		throw std::runtime_error(
			std::format("Failed to initialize decoder. Error code: {}", static_cast<int>(decoder_init_result)));
	}
	decoderInitialized = true;

	ma_device_config config = ma_device_config_init(ma_device_type_playback);
	config.playback.pDeviceID = &pAvailablePlaybackDevices[selectedDeviceIndex].id;
	config.playback.format = decoder.outputFormat;
	config.playback.channels = decoder.outputChannels;
	config.sampleRate = decoder.outputSampleRate;
	config.dataCallback = data_callback;
	config.pUserData = &decoder;
	
	ma_result result = ma_device_init(NULL, &config, &device);
	if (result != MA_SUCCESS) {
		throw std::runtime_error(
			std::format("Failed to initialize device. Error code: {}", static_cast<int>(result)));
	}
	deviceInitialized = true;

	ma_device_start(&device);
}