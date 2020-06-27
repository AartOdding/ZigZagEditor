#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include <ZigZag/ObjectSelection.hpp>

namespace ZigZag
{
    class Object;
}


class ObjectInspector
{
public:

    ObjectInspector(std::string_view windowName, ZigZag::Object* rootObject = nullptr);

    void setRootObject(ZigZag::Object* rootObject);

    void draw(bool* p_open);

private:

    void showObjectTree(ZigZag::Object* object);

    void clearUnusedObjectData();

    std::string m_windowName;
    
    ZigZag::Object* m_rootObject;
    ZigZag::Object* m_editedObject;

    ZigZag::ObjectSelection m_objectSelection;

    struct ObjectData
    {
        char name[64];
        std::string id;
        bool active;
    };

    std::unordered_map<ZigZag::Object*, ObjectData> m_objectData;

};
