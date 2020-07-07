#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include <ZigZag/Object.hpp>
#include <ZigZag/ObjectSelection.hpp>

#include "../app/ApplicationState.hpp"



class ObjectInspector
{
public:

    ObjectInspector(std::string_view windowName, ApplicationState* appState);

    void setRootObject(ZigZag::Object* rootObject);

    void draw(bool* p_open);

private:

    void showObjectTree(ZigZag::Object* object);

    void clearUnusedObjectData();

    std::string m_windowName;

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
