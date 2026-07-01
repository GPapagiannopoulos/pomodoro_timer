#include "Paths.h"
#include <vector>
#include <stdexcept>


#ifdef _WIN32
#include <windows.h>
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
#elif defined(__linux__)
std::filesystem::path Paths::executableDir() {
	return std::filesystem::read_symlink("/proc/self/exe").parent_path();
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
#endif