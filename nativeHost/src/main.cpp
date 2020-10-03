

#define ZIGZAG_IMPORT extern "C" __declspec(dllimport)


ZIGZAG_IMPORT void initialize();
ZIGZAG_IMPORT void render();
ZIGZAG_IMPORT void shutdown();
ZIGZAG_IMPORT bool shouldQuit();


int main()
{
	initialize();

	while (!shouldQuit())
	{
		render();
	}

	shutdown();
}
