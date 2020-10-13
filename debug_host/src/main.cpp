#include <cstdint>

#include <iostream>

#include <interop/MainloopInterop.hpp>
#include <interop/ObjectInterop.hpp>
#include <interop/ProjectInterop.hpp>


ZIGZAG_API void initialize();
ZIGZAG_API void render();
ZIGZAG_API void shutdown();
ZIGZAG_API bool shouldQuit();


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
