#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include <ZigZag/Object.hpp>
#include <ZigZag/ObjectSelection.hpp>

#include "app/ApplicationState.hpp"
#include "gui/Window.hpp"



class HierarchyWindow : public Window
{
public:

    HierarchyWindow(std::string_view windowName, ApplicationState* appState);

    void setScope(ZigZag::Object* rootObject);

protected:

    void draw() override;

private:

    void showObjectTree(ZigZag::Object* object);

    void clearUnusedObjectData();

    ApplicationState* m_appState;
    
    ZigZag::Object* m_rootObject{ nullptr };
    ZigZag::Object* m_editedObject{ nullptr };

    ZigZag::ObjectSelection m_objectSelection{ nullptr };
    ZigZag::CallbackId m_callbackId{ 0 };

    struct ObjectData
    {
        char name[64];
        bool active;
    };

    std::unordered_map<ZigZag::Object*, ObjectData> m_objectData;

};
