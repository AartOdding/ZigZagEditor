#include "Application.hpp"
#include "types.hpp"

#include "library/dataInputs/GenericInput.hpp"
#include "library/dataSources/TextureData.hpp"

#include <ZigZag/Object.hpp>
#include <ZigZag/BaseOperator.hpp>
#include <ZigZag/TParameter.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>


namespace zz = ZigZag;



Application::Application()
    : m_windowActions(std::make_shared<WindowActions>()),
      m_mainMenu(m_windowActions)
{
    //inspector.setRootObject(&op1);

    ///------------------------


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


}

void Application::draw()
{
    m_mainMenu.draw();
    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
    
    if (m_windowActions->viewportWindowOpen.getState())
    {
        bool open = true;
        m_viewport.draw(&open);
        m_windowActions->viewportWindowOpen.setState(open);
    }
    if (m_windowActions->objectInspectorWindowOpen.getState())
    {
        bool open = true;
        m_objectInspector.draw(&open);
        m_windowActions->objectInspectorWindowOpen.setState(open);
    }
    if (m_windowActions->imguiDemoWindowOpen.getState())
    {
        bool open = true;
        ImGui::ShowDemoWindow(&open);
        m_windowActions->imguiDemoWindowOpen.setState(open);
    }
    if (m_windowActions->imguiStyleWindowOpen.getState())
    {
        bool open = true;
        ImGui::Begin("Style Editor", &open); // Style editor is not inside of a window by defualt.
        ImGui::ShowStyleEditor();
        ImGui::End();
        m_windowActions->imguiStyleWindowOpen.setState(open);
    }
}
