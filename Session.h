#pragma once 
#include <chrono>
#include <string>
#include <string_view>

class Session {
private:
	std::chrono::minutes duration;
	std::string label;
	std::chrono::system_clock::time_point startedAt;
public:
	
	Session(std::chrono::minutes duration_, std::string_view label_);

	std::chrono::minutes getDuration() const;
	const std::string getLabel() const;
};