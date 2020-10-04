#include <cstdint>

#define ZIGZAG_IMPORT extern "C" __declspec(dllimport)


ZIGZAG_IMPORT void initialize();
ZIGZAG_IMPORT void render();
ZIGZAG_IMPORT void shutdown();
ZIGZAG_IMPORT bool shouldQuit();

ZIGZAG_IMPORT void onProjectCreated(const char* name, std::uint64_t projectID);


int main()
{
	initialize();

	onProjectCreated("project", 5001);

	while (!shouldQuit())
	{
		render();
	}

	shutdown();
}
