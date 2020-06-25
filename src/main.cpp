#include <iostream>

#include <ZigZag/Object.hpp>
#include <ZigZag/BaseOperator.hpp>
#include <ZigZag/TParameter.hpp>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <imgui_node_editor.h>

#include "library/dataInputs/GenericInput.hpp"
#include "library/dataSources/TextureData.hpp"

#include "types.hpp"
#include "gui/ObjectInspector.hpp"
#include "OpenSansRegular.hpp"


static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout << "framebuffer size: " << width << ", " << height << std::endl;
}

 
static void error_callback(int error, const char* description)
{
    std::cout << "Error:\n" << description << std::endl;
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

namespace zz = ZigZag;
namespace ImNode = ax::NodeEditor;

void menuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save", "CTRL+S")) {}
            if (ImGui::MenuItem("Open", "CTRL+O")) {}  // Disabled item
            if (ImGui::MenuItem("New", "CTRL+N")) {}  // Disabled item
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}


int main(int, char**)
{
    registerTypes();

    auto i1 = zz::ObjectFactory::instance()->create("IntParameter");
    std::cout << i1->typeName() << std::endl;

    zz::BaseOperator op1(nullptr, "op1");
    IntParameter par1{ &op1 };
    IntParameter par2{ &op1 };

    TextureData tex1(nullptr, "text1");
    GenericInput<TextureData> textIn1(nullptr, "texIn1");

    std::cout << par1.typeName() << std::endl;
    std::cout << par1.value() << " " << par2.value() << std::endl;

    connect(&par1, &par2);

    //par1.consume(100);
    par1 = 100;
    std::cout << par1.value() << " " << par2.value() << std::endl;

    par2.process();
    //par1.process();

    std::cout << par1.value() << " " << par2.value() << std::endl;

    //par2.process();

    //std::cout << par1.value() << " " << par2.value() << std::endl;

    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
    {
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(1280, 720, "ZigZag", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGL())
    {
        return 1;
    }
    
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.IniFilename = nullptr;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    const char* glsl_version = "#version 150";

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    ImFontConfig fontConfig;
    fontConfig.OversampleH = 4;
    fontConfig.OversampleV = 4;
    fontConfig.FontDataOwnedByAtlas = false;
    io.Fonts->AddFontFromMemoryTTF((void*)OpenSans_Regular_ttf, OpenSans_Regular_ttf_len, 18, &fontConfig);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ObjectInspector inspector{ "Inspector" };
    inspector.setRootObject(&op1);

    ImNode::Config cfg;
    cfg.SettingsFile = nullptr;
    auto nodeContext = ImNode::CreateEditor(&cfg);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImNode::SetCurrentEditor(nodeContext);

        //ImGui::SetCursorPos({0, 0});
        //
        ImGuiWindowFlags f = ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
            | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar
            | ImGuiWindowFlags_NoBringToFrontOnFocus;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize(ImVec2(width, height));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
        ImGui::Begin("Network", 0, f);
        
        menuBar();

        ImNode::Begin("My Editor", { 0, 0 });

        int uniqueId = 1;

        // Start drawing nodes.
        ImNode::BeginNode(uniqueId++);
        ImGui::Text("Node A");
        ImNode::BeginPin(uniqueId++, ImNode::PinKind::Input);
        ImGui::Text("-> In");
        ImNode::EndPin();
        ImGui::SameLine();
        ImNode::BeginPin(uniqueId++, ImNode::PinKind::Output);
        ImGui::Text("Out ->");
        ImNode::EndPin();
        ImNode::EndNode();

        //ImNode::NavigateToContent();

        // Start drawing nodes.
        ImNode::BeginNode(uniqueId++);
        ImGui::Text("Node B");
        ImNode::BeginPin(uniqueId++, ImNode::PinKind::Input);
        ImGui::Text("-> In");
        ImNode::EndPin();
        ImGui::SameLine();
        ImNode::BeginPin(uniqueId++, ImNode::PinKind::Output);
        ImGui::Text("Out ->");
        ImNode::EndPin();
        ImNode::EndNode();

        ImNode::End();

        ImGui::End();
        ImGui::PopStyleVar(1);

        ImGui::ShowDemoWindow(&show_demo_window);

        inspector.draw();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImNode::DestroyEditor(nodeContext);

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
/*

int main()
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        throw "error";
    }
 
    glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
 
  
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();

    ZigZag::Object obj(nullptr, "hay");
    std::cout << "hello" << std::endl;

    return 0;
}
*/