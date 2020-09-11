#include <app/Directories.hpp>
#include <ZigZag/Platform.hpp>

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#ifdef ZIGZAG_UNIX_LIKE
	#include <pwd.h>
	#include <unistd.h>
#endif


namespace Directories
{
	std::filesystem::path homeDir()
	{
		static bool initialized = false;
		static std::string home;

		if (!initialized)
		{
#if defined(ZIGZAG_WINDOWS)
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
#elif defined(ZIGZAG_UNIX_LIKE)
			auto p = getpwuid(getuid());
			home = p ? p->pw_dir : getenv("HOME");
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

	std::filesystem::path settingsDir()
	{
		static bool initialized = false;
		static std::string settings;

		if (!initialized)
		{
#if defined(ZIGZAG_WINDOWS)
			settings = homeDir().string();
			settings.append("\\AppData\\Local\\ZigZag");
#elif defined(ZIGZAG_MAC_OSX)
			settings = homeDir();
			settings.append("/Library/Preferences/ZigZag");
#elif defined (ZIGZAG_LINUX)
			static_assert(false);
			//TODO find proper location for linux
#endif
			initialized = true;
		}
		return settings.c_str();
	}


	std::filesystem::path resourcesDir()
	{
		static bool initialized = false;
		static std::string resources;

		if (!initialized)
		{
			auto path = std::filesystem::current_path();
			auto resourcesPath = path / "resources";

			while (path != path.parent_path() && !std::filesystem::exists(resourcesPath))
			{
				path = path.parent_path();
				resourcesPath = path / "resources";
			}

			if (std::filesystem::exists(resourcesPath))
			{
				if (std::filesystem::exists(resourcesPath / "ZigZagResourcesTag.txt"))
				{
					resources = resourcesPath.string();
				}
			}
			if (resources.empty())
			{
				std::cerr << "ZigZag could not start up properly, because the resources directory could not be detected." << std::endl;
				throw std::runtime_error("Could not detect resources directory.");
			}
			initialized = true;
		}
		return resources.c_str();
	}

	void createSettingsDir()
	{
		// If the path already exists nothing happens.
		std::filesystem::create_directory(settingsDir());
	}
}

