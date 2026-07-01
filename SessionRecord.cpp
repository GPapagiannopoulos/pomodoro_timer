#include "SessionRecord.h"
#include <chrono>

SessionRecord::SessionRecord(const Session* session) {
	intendedDuration = session->getDuration().count();
	label = session->getLabel();
	startedAt = session->getStartTime();
	isCompleted = session->isTimerComplete();
}