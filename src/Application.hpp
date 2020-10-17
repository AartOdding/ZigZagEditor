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

#include <object/ObjectTypeNamespace.hpp>
#include <object/ZObject.hpp>

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
    
    ZObject* getRootObject();
    const ZObject* getRootObject() const;
    void setRootObject(std::unique_ptr<ZObject>&& object);
    void clearRootObject();

    float e() const;

    ObjectTypeNamespace* getRootTypeNamespace();

private:

    Application();

    ApplicationState m_appState;

    MainMenu m_mainMenu{ this, &m_appState };

    HierarchyWindow m_hierarchyWindow{ "Project Hierarchy", this, &m_appState };
    HistoryWindow m_historyWindow{ "History", &m_appState };
    NodeEditorWindow m_nodeEditorWindow{ "Node Editor", &m_appState };
    RenderOrderWindow m_renderOrderWindow{ "Render Order", &m_appState };
    StyleEditorWindow m_styleEditorWindow{ "Style Editor", &m_appState };

    std::unordered_map<ZigZag::LuaBehaviour*, std::unique_ptr<LuaEditorWindow>> m_luaEditorWindows;

    float m_widthOfE = 7;
    bool m_ImGuiDemoWindowOpen = false;
    bool m_ImGuiStyleEditorWindowOpen = false;

    std::unique_ptr<ZObject> m_rootObject;
    std::unique_ptr<ObjectTypeNamespace> m_rootTypeNamespace;

};
