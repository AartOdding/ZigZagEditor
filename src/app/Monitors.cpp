#include <app/Monitors.hpp>

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Monitors
{
	void initialize()
	{
		std::cout << "init monitors" << std::endl;

        int monitorCount;
        auto monitors = glfwGetMonitors(&monitorCount);

        for (int i = 0; i < monitorCount; ++i)
        {
            int w, h;
            float sx, sy;
            auto name = glfwGetMonitorName(monitors[i]);
            auto videoMode = glfwGetVideoMode(monitors[i]);
            glfwGetMonitorPhysicalSize(monitors[i], &w, &h);
            glfwGetMonitorContentScale(monitors[i], &sx, &sy);

            std::cout << "\nMonitor: " << name << "\n";
            std::cout << "Resolution: " << videoMode->width << ", " << videoMode->height << "\n";
            std::cout << "Physical size: " << w << "mm, " << h << "mm\n";
            std::cout << "Scaling: " << sx << ", " << sy << "\n";
            std::cout << "Refresh Rate: " << videoMode->refreshRate << std::endl;
        }

	}
}