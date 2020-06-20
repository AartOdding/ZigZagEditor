#pragma once

#include <string>
#include <string_view>
#include <unordered_set>

namespace ZigZag
{
    class Object;
}


class ObjectInspector
{
public:

    ObjectInspector(std::string_view windowName, ZigZag::Object* rootObject = nullptr);

    void setRootObject(ZigZag::Object* rootObject);

    void draw();

private:

    void showObjectTree(ZigZag::Object* object);

    ZigZag::Object* m_rootObject;
    ZigZag::Object* m_editedObject;
    
    std::string m_windowName;

    std::unordered_set<ZigZag::Object*> m_selectedObjects;

};
