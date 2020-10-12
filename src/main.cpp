#include <cstdlib>
#include <iostream>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Application.hpp"

#include <app/Directories.hpp>
#include <app/style/FontLibrary.hpp>

#include <ZigZag/Platform.hpp>

#define ZIGZAG_EXPORT extern "C" __declspec(dllexport)


namespace // Unnamed namespace keeps everything inside local to this file.
{

    float getFramebufferScaling(GLFWwindow* window)
    {
        int sizeX, sizeY, fbSizeX, fbSizeY;
        glfwGetWindowSize(window, &sizeX, &sizeY);
        glfwGetFramebufferSize(window, &fbSizeX, &fbSizeY);
        return static_cast<float>(fbSizeX) / static_cast<float>(sizeX);
    }

    GLFWwindow* window = nullptr;
} 


ZIGZAG_EXPORT GLFWglproc ZigZagGetProcAddress(const char* procName)
{
    return glfwGetProcAddress(procName);
}


ZIGZAG_EXPORT void initialize()
{
    Directories::createSettingsDir();

    glfwSetErrorCallback([](int errorCode, const char* errorDescription) {
        std::cerr << "GLFW error " << errorCode << ": " << errorDescription << std::endl;
    });

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        std::exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_DECORATED, true);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, true);

    window = glfwCreateWindow(1280, 720, "ZigZag", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create window." << std::endl;
        glfwTerminate();
        std::exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    glfwSetWindowContentScaleCallback(window, [](GLFWwindow* window, float scaleX, float scaleY) {
        auto fontLibrary = &(Application::getGlobalInstance()->getAppState()->fontLibrary);
        if (fontLibrary)
        {
            fontLibrary->setScaling(scaleX, getFramebufferScaling(window));
        }
        std::cout << "window scale changed: " << scaleX << std::endl;
    });

    if (!gladLoadGL())
    {
        std::cerr << "Failed to load OpenGL." << std::endl;
        glfwTerminate();
        std::exit(1);
    }

    ImGuiContext* imguiContext = ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    float sx, sy;
    glfwGetWindowContentScale(window, &sx, &sy);

    auto fontLibrary = &(Application::getGlobalInstance()->getAppState()->fontLibrary);
    fontLibrary->setScaling(sx, getFramebufferScaling(window));
}

ZIGZAG_EXPORT void render()
{
    auto fontLibrary = &(Application::getGlobalInstance()->getAppState()->fontLibrary);
    fontLibrary->updateFonts();

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Application::getGlobalInstance()->update();

    ImGui::Render();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

ZIGZAG_EXPORT void shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

ZIGZAG_EXPORT bool shouldQuit()
{
    return glfwWindowShouldClose(window);
}
