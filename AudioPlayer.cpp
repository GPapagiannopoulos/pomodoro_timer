#include "AudioPlayer.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdexcept>
#include <format>

AudioPlayer::AudioPlayer() {
	ma_result result = ma_context_init(NULL, 0, NULL, &this->context);
	if (result != MA_SUCCESS) {
		throw std::runtime_error(
			std::format("Failed to initialize playback context. Error code: {}", static_cast<int>(result)));
	}
}

AudioPlayer::~AudioPlayer() {
	ma_context_uninit(&this->context);
}