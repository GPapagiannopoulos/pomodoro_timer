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
	if (deviceInitialized) {
		ma_device_uninit(&device);
	}
	if (decoderInitialized) {
		ma_decoder_uninit(&decoder);
	}
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
ma_decoder* AudioPlayer::getDecoder()  {
	return &decoder;
}
bool AudioPlayer::getLoopSound() const {
	return loopSound;
}
void AudioPlayer::updateLoopSound() {
	loopSound = !loopSound;
}
bool AudioPlayer::getAlertStatus() const {
	return alertPlayed;
}
void AudioPlayer::updateAlertStatus() {
	mutex.lock();
	alertPlayed = !alertPlayed;
	mutex.unlock();
	c_var.notify_one();
}
void AudioPlayer::uninitDevice() {
	ma_device_uninit(&device);
	deviceInitialized = false;
}
void AudioPlayer::uninitDecoder() {
	ma_decoder_uninit(&decoder);
	decoderInitialized = false;
}
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	AudioPlayer* player = static_cast<AudioPlayer*>(pDevice->pUserData);
	ma_decoder* decoder = player->getDecoder();
	ma_uint32 bytesPerFrame = ma_get_bytes_per_frame(pDevice->playback.format, pDevice->playback.channels);
	ma_uint64 framesWritten = 0;
	uint8_t* pOutputOffset = static_cast<uint8_t*>(pOutput);

	while (framesWritten < frameCount) {
		ma_uint64 framesWrittenThisIteration = 0;
		ma_decoder_read_pcm_frames(decoder, pOutputOffset, frameCount - framesWritten, &framesWrittenThisIteration);
		framesWritten += framesWrittenThisIteration;
		pOutputOffset += framesWrittenThisIteration * bytesPerFrame;

		if (framesWritten < frameCount) {
			if (!player->getLoopSound()) {
				player->updateAlertStatus();
				return;
			}
			ma_decoder_seek_to_pcm_frame(decoder, 0);
		}
	}
}

void AudioPlayer::initializeDecoder(std::string file_path) {
	ma_result decoder_init_result = ma_decoder_init_file(file_path.c_str(), NULL, &decoder);
	if (decoder_init_result != MA_SUCCESS) {
		throw std::runtime_error(
			std::format("Failed to initialize decoder. Error code: {}", static_cast<int>(decoder_init_result)));
	}
	decoderInitialized = true;
}
void AudioPlayer::initializePlaybackDevice() {
	ma_device_config config = ma_device_config_init(ma_device_type_playback);
	config.playback.pDeviceID = &pAvailablePlaybackDevices[selectedDeviceIndex].id;
	config.playback.format = decoder.outputFormat;
	config.playback.channels = decoder.outputChannels;
	config.sampleRate = decoder.outputSampleRate;
	config.dataCallback = data_callback;
	config.pUserData = this;

	ma_result result = ma_device_init(NULL, &config, &device);
	if (result != MA_SUCCESS) {
		throw std::runtime_error(
			std::format("Failed to initialize device. Error code: {}", static_cast<int>(result)));
	}
	deviceInitialized = true;
}

void AudioPlayer::playAlert() {
	if (getAlertStatus()) {
		updateAlertStatus();
	}
	initializeDecoder("C:/Users/georg/source/repos/pomodoro_timer/sounds/alerts/clear_bell_chime.mp3");
	initializePlaybackDevice();
	ma_device_start(&device);
	std::unique_lock<std::mutex> lock (mutex);
	c_var.wait(lock, [this] { return getAlertStatus(); });

	ma_device_stop(&device);
	uninitDevice();
	uninitDecoder();
}

void AudioPlayer::playAmbientNoise() {
	updateLoopSound();
	initializeDecoder("C:/Users/georg/source/repos/pomodoro_timer/sounds/brown_noise/smooth_brown_noise.mp3");
	initializePlaybackDevice();
	ma_device_start(&device);
}

void AudioPlayer::stopAudioDevice() {
	ma_device_stop(&device);
	uninitDevice();
	uninitDecoder();
	
	updateLoopSound();
	playAlert();
}