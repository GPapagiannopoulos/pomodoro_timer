#pragma once 
#include <chrono>
#include <string>
#include "Session.h"

class SessionRecord {
private:
	int intendedDuration;
	std::string label;
	std::chrono::system_clock::time_point startedAt;
	std::optional<std::chrono::system_clock::time_point> endedAt; // used only for read operations
	bool isCompleted = false;
public:
	SessionRecord(const Session* session);
};