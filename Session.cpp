#include "Session.h"

Session::Session(std::chrono::minutes duration_, std::string_view label_)
	: duration{ duration_ }, label{ label_ } {
	startedAt = std::chrono::system_clock::now();
}

std::chrono::minutes Session::getDuration() const {
	return duration;
}
const std::string Session::getLabel() const {
	return label;
}