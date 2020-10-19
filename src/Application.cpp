#include "Application.hpp"
#include "types.hpp"

#include "app/Directories.hpp"

#include <ZigZag/Object.hpp>
#include <ZigZag/BaseOperator.hpp>
#include <ZigZag/LuaBehaviour.hpp>
#include <ZigZag/TParameter.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <gui/popups/OperatorSelectionPopup.hpp>

#include <iostream>


namespace zz = ZigZag;



Application* Application::getGlobalInstance()
{
    static Application globalApplication;
    return &globalApplication;
}

Application::Application()
{
    registerTypes();
    ZigZag::LuaBehaviour::loadDefaultScript((Directories::resourcesDir() / "LuaBehaviourDefaultScript.lua").string());

    m_rootNodeTemplateGroup = NodeTemplateGroup::create("");

    m_hierarchyWindow.setScope(&m_appState.rootOperator);
}

void Application::update()
{
    ImGui::PushFont(m_appState.fontLibrary.getUiFont());
    m_widthOfE = ImGui::CalcTextSize("e").x;

    m_appState.style.push("Application");

    m_mainMenu.draw();
    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
    
    m_nodeEditorWindow.update();
    m_hierarchyWindow.update();
    m_historyWindow.update();
    m_renderOrderWindow.update();
    m_styleEditorWindow.update();

    for (auto it = m_luaEditorWindows.begin(); it != m_luaEditorWindows.end();) // Important: no increment
    {
        it->second->update();
        it->second->isOpen() ? ++it : it = m_luaEditorWindows.erase(it);
    }

    if (m_ImGuiDemoWindowOpen)
    {
        ImGui::ShowDemoWindow(&m_ImGuiDemoWindowOpen);
    }
    if (m_ImGuiStyleEditorWindowOpen)
    {
        ImGui::Begin("Imgui Style Editor", &m_ImGuiStyleEditorWindowOpen);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }

    m_appState.style.pop("Application");
    ImGui::PopFont();
}

void Application::openWindow(WindowType type)
{
    switch (type)
    {
        case WindowType::NodeEditor: 
            m_nodeEditorWindow.open();
            break;
        case WindowType::Hierarchy: 
            m_hierarchyWindow.open();
            break;
        case WindowType::History: 
            m_historyWindow.open();
            break;
        case WindowType::RenderOrder: 
            m_renderOrderWindow.open();
            break;
        case WindowType::StyleEditor: 
            m_styleEditorWindow.open();
            break;
        case WindowType::ImGuiDemo: 
            m_ImGuiDemoWindowOpen = true;
            break;
        case WindowType::ImGuiStyleEditor: 
            m_ImGuiStyleEditorWindowOpen = true;
            break;
    }
}

void Application::openLuaEditorWindow(ZigZag::LuaBehaviour* luaBehaviour)
{
    if (luaBehaviour)
    {
        auto it = m_luaEditorWindows.find(luaBehaviour);

        if (it != m_luaEditorWindows.end())
        {
            it->second->open();
            // also set focus
        }
        else
        {
            m_luaEditorWindows[luaBehaviour] = std::make_unique<LuaEditorWindow>(*luaBehaviour, &m_appState);
        }
    }
}

int Application::windowOpenCount(WindowType type)
{
    switch (type)
    {
        case WindowType::NodeEditor: return m_nodeEditorWindow.isOpen();
        case WindowType::Hierarchy: return m_hierarchyWindow.isOpen();
        case WindowType::History: return m_historyWindow.isOpen();
        case WindowType::RenderOrder: return m_renderOrderWindow.isOpen();
        case WindowType::StyleEditor: return m_styleEditorWindow.isOpen();
        case WindowType::LuaEditor: return m_luaEditorWindows.size();
        case WindowType::ImGuiDemo: return m_ImGuiDemoWindowOpen;
        case WindowType::ImGuiStyleEditor: return m_ImGuiStyleEditorWindowOpen;
    }
    return 0;
}

ApplicationState* Application::getAppState()
{
    return &m_appState;
}

Node* Application::getRootObject()
{
    return m_rootNode.get();
}

const Node* Application::getRootObject() const
{
    return m_rootNode.get();
}

void Application::setRootObject(std::unique_ptr<Node>&& object)
{
    m_rootNode = std::move(object);
    m_nodeEditorWindow.setScope(m_rootNode->getIdentifier());
}

void Application::clearRootObject()
{
    m_rootNode.reset();
}

float Application::e() const
{
    return m_widthOfE;
}

NodeTemplateGroup* Application::getRootTypeNamespace()
{
    return m_rootNodeTemplateGroup.get();
}

void Application::addParentlessNode(Node::Pointer&& node)
{
    std::cout << "[editor dll] Storing parentless node in Application: " << node->getIdentifier() << std::endl;
    if (node)
    {
        m_parentlessNodes[node->getIdentifier()] = std::move(node);
    }
}

Node::Pointer Application::takeParentlessNode(Identifier<Node> nodeID)
{
    std::cout << "[editor dll] Removing parentless node from Application: " << nodeID << std::endl;
    auto it = m_parentlessNodes.find(nodeID);
    if (it != m_parentlessNodes.end())
    {
        auto node = std::move(it->second);
        m_parentlessNodes.erase(it);
        return std::move(node);
    }
    return Node::Pointer();
}

bool Application::existsParentlessNode(Identifier<Node> nodeID)
{
    return m_parentlessNodes.find(nodeID) != m_parentlessNodes.end();
}
