#include "SessionStore.h"
#include <string>
#include <stdexcept>
#include <format>

SessionStore::SessionStore(const std::string& filename) {
	int dbOpenResult = sqlite3_open_v2(filename.c_str(), &dbHandle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	if (dbOpenResult != SQLITE_OK) {
		sqlite3_close(dbHandle);
		throw std::runtime_error(
			std::format("Opening of database file failed. Error: {}", dbOpenResult));
	}
	char* errorMsg = nullptr;
	int dbWriteResult = sqlite3_exec(dbHandle, "CREATE TABLE IF NOT EXISTS sessions (id INTEGER PRIMARY KEY AUTOINCREMENT, label TEXT NOT NULL, intended_duration INTEGER NOT NULL, started_at INTEGER NOT NULL, ended_at INTEGER, is_completed INTEGER NOT NULL DEFAULT 0);", NULL, NULL, &errorMsg);
	if (dbWriteResult != SQLITE_OK) {
		std::string errorMsgStr(errorMsg);
		sqlite3_free(errorMsg);
		throw std::runtime_error(errorMsgStr);
	}
}

SessionStore::~SessionStore() {
	if (dbHandle) {
		sqlite3_close(dbHandle);
	}
}