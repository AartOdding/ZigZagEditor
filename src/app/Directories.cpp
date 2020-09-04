#include <app/Directories.hpp>

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	// No extra includes necessary for windows
#elif defined(__APPLE__) || defined(__linux__)
	#include <pwd.h>
	#include <unistd.h>
#else
	#error "Platform not supported."
#endif



namespace Directories
{
	const char* homeDir()
	{
		static bool initialized = false;
		static std::string home;

		if (!initialized)
		{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
			const auto userProfile = std::getenv("USERPROFILE");
			if (userProfile)
			{
				home = userProfile;
			}
			else
			{
				const auto homeDrive = std::getenv("HOMEDRIVE");
				const auto homePath = std::getenv("HOMEPATH");
				if (homeDrive && homePath)
				{
					home = homeDrive;
					home.append(homePath);
				}
			}
#elif defined(__APPLE__) || defined(__linux__)

			auto p = getpwuid(getuid());
			homeDir = p ? p->pw_dir : getenv("HOME");
#endif
			if (home.empty())
			{
				std::cerr << "ZigZag could not start up properly, because the home directory could not be detected." << std::endl;
				throw std::runtime_error("Could not detect home directory.");
			}
			initialized = true;
		}
		return home.c_str();
	}

	const char* settingsDir()
	{
		static bool initialized = false;
		static std::string settings;

		if (!initialized)
		{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
			settings = homeDir();
			settings.append("\\AppData\\Local\\ZigZag");
#elif defined(__APPLE__)
			settingsDir = home();
			settingsDir.append("/Library/Preferences/ZigZag");
#elif defined (__linux__)

#endif
			initialized = true;
		}
		return settings.c_str();
	}

	void createSettingsDir()
	{
		// If the path already exists nothing happens.
		std::filesystem::create_directory(settingsDir());
	}
}

