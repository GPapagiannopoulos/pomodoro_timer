#include <iostream>
#include "Session.h"

int main() {
	
	Session test_session = Session(std::chrono::minutes(15), "Test");

	std::cout << test_session.getDuration() << "\n";
	std::cout << test_session.getLabel() << "\n";

	return 0;
}