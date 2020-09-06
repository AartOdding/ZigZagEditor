#include <filesystem>


namespace Directories
{
	std::filesystem::path homeDir();
	std::filesystem::path settingsDir();
	std::filesystem::path resourcesDir();
	void createSettingsDir();
}
