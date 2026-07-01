#include "Paths.h"
#include <vector>
#include <stdexcept>


#ifdef _WIN32
#include <windows.h>
#include <cstdlib>
std::filesystem::path Paths::executableDir() {
	std::vector<wchar_t> buffer(MAX_PATH);
	DWORD charsWritten = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
	if (charsWritten == 0) throw std::runtime_error("Failed to get module name.");

	while (charsWritten == buffer.size()) {
		const size_t newSize = buffer.size() * 2;
		buffer.resize(newSize);
		charsWritten = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
		if (charsWritten == 0) throw std::runtime_error("Failed to get module name.");
	}
	return std::filesystem::path(buffer.data()).parent_path();
}

std::filesystem::path Paths::userDataDir(const std::string& appName) {
	char* appdata = std::getenv("APPDATA");
	if (!appdata) throw std::runtime_error("Failed to extract app data.");
	std::filesystem::path dir = std::filesystem::path(appdata) / appName;
	std::filesystem::create_directories(dir);
	return dir;
}

#elif defined(__linux__)
#include <cstring>
std::filesystem::path Paths::executableDir() {
	return std::filesystem::read_symlink("/proc/self/exe").parent_path();
}
std::filesystem::path Paths::userDataDir(const std::string& appName) {
	char* xdgDataHome = std::getenv("XDG_DATA_HOME");
	std::filesystem::path dir;
	if (!xdgDataHome || std::strlen(xdgDataHome) == 0) {
		char* homeDir = std::getenv("HOME");
		if (!homeDir || std::strlen(homeDir) == 0) throw std::runtime_error("Failed to extract app data.");
		dir = std::filesystem::path(homeDir) / ".local" / "share" / appName;
	}
	else {
		dir = std::filesystem::path(xdgDataHome) / appName;
	}
	std::filesystem::create_directories(dir);
	return dir;
}

#elif defined(__APPLE__)
#include <mach-o/dyld.h>
std::filesystem::path Paths::executableDir() {
	uint32_t bufferSize = 64;
	std::vector<char> buffer(bufferSize);
	int writeResult = _NSGetExecutablePath(buffer.data(), &bufferSize);
	if (writeResult == -1) {
		buffer.resize(bufferSize);
		int secondTry = _NSGetExecutablePath(buffer.data(), &bufferSize);
		if (secondTry == -1) {
			throw std::runtime_error("Failed to get module name.");
		}
	} 
	return std::filesystem::canonical(std::filesystem::path(buffer.data()).parent_path());
}
std::filesystem::path Paths::userDataDir(const std::string& appName) {
	char* home = std::getenv("HOME");
	if (!home) throw std::runtime_error("Failed to get home directory.");
	std::filesystem::path dir = std::filesystem::path(home) / "Library/Application Support" / appName;
	std::filesystem::create_directories(dir);
	return dir;
}
#endif