#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include <ZigZag/Object.hpp>
#include <ZigZag/ObjectSelection.hpp>

#include "gui/Window.hpp"


class Application;
class ApplicationState;


class HierarchyWindow : public Window
{
public:

    HierarchyWindow(std::string_view windowName, Application* app, ApplicationState* appState);

    void setScope(ZigZag::Object* rootObject);

protected:

    void draw() override;

private:

    void showObjectTree(ZigZag::Object* object);

    void contextMenu(ZigZag::Object* object);

    void clearUnusedObjectData();

    Application* m_application;
    ApplicationState* m_appState;
    
    ZigZag::Object* m_rootObject{ nullptr };
    ZigZag::Object* m_editedObject{ nullptr };

    ZigZag::ObjectSelection m_objectSelection{ nullptr };

    struct ObjectData
    {
        char name[64];
        bool active;
    };

    std::unordered_map<ZigZag::Object*, ObjectData> m_objectData;

};
