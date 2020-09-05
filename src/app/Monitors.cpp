#include <app/Monitors.hpp>

#include <cmath>
#include <iostream>
#include <unordered_map>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace
{
    struct Scale
    {
        Scale() = default;
        Scale(float sx, float sy) : x(sx), y(sy) { }
        float x = 1.0f;
        float y = 1.0f;
    };

    bool operator==(const Scale& lhs, const Scale& rhs)
    {
        return std::fabs(lhs.x - rhs.x) < 0.00001 && std::fabs(lhs.y - rhs.y) < 0.00001;
    }
    bool operator!=(const Scale& lhs, const Scale& rhs)
    {
        return !(lhs == rhs);
    }

    std::unordered_map<GLFWwindow*, Scale> windowScales;
}

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

    void trackWindow(GLFWwindow* window)
    {
        windowScales.emplace(std::make_pair(window, Scale()));

        auto scaleCallback = [](GLFWwindow* window, float sx, float sy)
        {
            auto oldScale = windowScales[window];
            if (oldScale != Scale(sx, sy))
            {
                int width, height;
                glfwGetWindowSize(window, &width, &height);
                glfwSetWindowSize(window, width * (sx / oldScale.x), height * (sy / oldScale.y));
                windowScales[window] = Scale(sx, sy);
            }
        };

        float sx, sy;
        glfwGetWindowContentScale(window, &sx, &sy);
        scaleCallback(window, sx, sy);

        glfwSetWindowContentScaleCallback(window, scaleCallback);

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
        {
            windowScales.erase(window);
        });
    }


}