#pragma once 

#include <string>
#include <sqlite3.h>

class SessionStore {
private:
	sqlite3 *dbHandle;
public:
	SessionStore(const std::string& filename);
	~SessionStore();
};