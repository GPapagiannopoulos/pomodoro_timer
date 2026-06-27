#include "Session.h"
#include <iostream>

Session::Session(std::chrono::minutes duration_, std::string_view label_)
	: duration{ duration_ }, label{ label_ } {
	startedAt = std::chrono::system_clock::now();
}

std::chrono::minutes Session::getDuration() const {
	return duration;
}

std::tuple<int, int, int> Session::getRemainingTime() const {
	const std::chrono::duration time_passed = getDuration() - (std::chrono::system_clock::now() - startedAt);
	int remaining_hours = std::chrono::duration_cast<std::chrono::hours>(time_passed).count();
	int remaining_minutes = std::chrono::duration_cast<std::chrono::minutes>(time_passed).count() % 60;
	int remaining_seconds = std::chrono::duration_cast<std::chrono::seconds>(time_passed).count() % 60; 

	return std::make_tuple(remaining_hours, remaining_minutes, remaining_seconds);
}
const std::string Session::getLabel() const {
	return label;
}
bool Session::isTimerComplete() const {
	auto [h, m, s] = getRemainingTime();
	return h == 0 && m == 0 && s == 0;
}