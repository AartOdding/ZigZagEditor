#pragma once

#include <memory>
#include <unordered_map>

#include "app/ApplicationState.hpp"

#include "gui/HierarchyWindow.hpp"
#include "gui/HistoryWindow.hpp"
#include "gui/LuaEditorWindow.hpp"
#include "gui/MainMenu.hpp"
#include "gui/NodeEditorWindow.hpp"
#include "gui/RenderOrderWindow.hpp"
#include "gui/StyleEditorWindow.hpp"

#include <object/Node.hpp>
#include <object/NodeTemplateGroup.hpp>

namespace ZigZag
{
    class LuaBehaviour;
}

class Application
{
public:

    enum class WindowType
    {
        NodeEditor,
        Hierarchy,
        History,
        LuaEditor,
        RenderOrder,
        StyleEditor,
        ImGuiDemo,
        ImGuiStyleEditor
    };

    static Application* getGlobalInstance();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    void update();

    void openWindow(WindowType type);
    void openLuaEditorWindow(ZigZag::LuaBehaviour* luaBehaviour);

    int windowOpenCount(WindowType type);

    ApplicationState* getAppState();
    
    Node* getRootObject();
    const Node* getRootObject() const;
    void setRootObject(std::unique_ptr<Node>&& object);
    void clearRootObject();

    float e() const;

    NodeTemplateGroup* getRootTypeNamespace();

    void addParentlessNode(Node::Pointer&& node);
    Node::Pointer takeParentlessNode(Identifier<Node> nodeID);
    bool existsParentlessNode(Identifier<Node> nodeID);

private:

    Application();

    ApplicationState m_appState;

    MainMenu m_mainMenu{ this, &m_appState };

    HierarchyWindow m_hierarchyWindow{ "Project Hierarchy", this, &m_appState };
    HistoryWindow m_historyWindow{ "History", &m_appState };
    NodeEditorWindow m_nodeEditorWindow{ "Node Editor" };
    RenderOrderWindow m_renderOrderWindow{ "Render Order", &m_appState };
    StyleEditorWindow m_styleEditorWindow{ "Style Editor", &m_appState };

    std::unordered_map<ZigZag::LuaBehaviour*, std::unique_ptr<LuaEditorWindow>> m_luaEditorWindows;

    float m_widthOfE = 7;
    bool m_ImGuiDemoWindowOpen = false;
    bool m_ImGuiStyleEditorWindowOpen = false;

    std::unique_ptr<Node> m_rootNode;
    std::unordered_map<Identifier<Node>, Node::Pointer> m_parentlessNodes;

    std::unique_ptr<NodeTemplateGroup> m_rootNodeTemplateGroup;

};
