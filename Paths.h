#pragma once
#include <filesystem>
#include <string>

namespace Paths {
	std::filesystem::path executableDir();
	std::filesystem::path userDataDir(const std::string& appName);
}