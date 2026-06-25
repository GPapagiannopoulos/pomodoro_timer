#include <iostream>
#include <thread>

#include "Session.h"

int main() {
	
	Session test_session = Session(std::chrono::minutes(15), "Test");

	std::cout << test_session.getDuration() << "\n";
	std::cout << test_session.getLabel() << "\n";
	std::this_thread::sleep_for(std::chrono::seconds(10));
	auto [hours, minutes, seconds] = test_session.getRemainingTime();
	std::cout << hours << " " << minutes << " " << seconds << "\n";
	return 0;
}