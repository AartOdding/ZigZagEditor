#include "ObjectInspector.hpp"

#include <imgui.h>

#include <ZigZag/Object.hpp>
#include <ZigZag/ObjectFactory.hpp>
#include <iostream>

using namespace ImGui;
using namespace ZigZag;


ObjectInspector::ObjectInspector(std::string_view windowName, ZigZag::Object* rootObject)
    : m_windowName(windowName),
      m_rootObject(rootObject)
{
}


void ObjectInspector::setRootObject(ZigZag::Object* rootObject)
{
    m_rootObject = rootObject;
}


void ObjectInspector::draw()
{
    Begin(m_windowName.c_str());


    if (Button("Add Child"))
    {
        if (m_selectedObjects.size() == 1)
        {
            m_editedObject = *m_selectedObjects.begin();
            OpenPopup("Select object type");
        }
    }
    if (BeginPopup("Select object type"))
    {
        int current = -1;
        auto factory = ObjectFactory::instance();
        ListBoxHeader("test");

        for (const auto& type : factory->getObjectTypeNames())
        {
            Selectable(type.c_str(), false);
            if (IsItemClicked())
            {
                auto newObject = factory->create(type);
                newObject->setParent(m_editedObject);
                std::cout << type << std::endl;
            }
        }

        ListBoxFooter();
        //ListBox("test", &current, factory->getObjectNamesCArray(), factory->getObjectCount(), 10);
        //if (IsItemClicked())
        //{
        //    std::cout << current << std::endl;
        //}
        //ListBoxHeader("test", 20, 12);
        //ListBoxFooter();
        EndPopup();
    }

    Columns(2);

    showObjectTree(m_rootObject);

    End();
}


void ObjectInspector::showObjectTree(ZigZag::Object* object)
{
    if (object)
    {
        //AlignTextToFramePadding();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
        flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
        flags |= ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanFullWidth;
        flags |= ImGuiTreeNodeFlags_AllowItemOverlap;
        flags |= m_selectedObjects.count(object) ? ImGuiTreeNodeFlags_Selected : 0;
        flags |= object->getChildObjects().empty() ? ImGuiTreeNodeFlags_Leaf : 0;
        flags |= object->getChildObjects().empty() ? ImGuiTreeNodeFlags_Bullet : 0;

        bool open = TreeNodeEx(object->typeName(), flags);

        if (IsItemClicked())
        {
            if (GetIO().KeyCtrl)
            {
                if (m_selectedObjects.count(object))
                {
                    m_selectedObjects.erase(object);
                }
                else
                {
                    m_selectedObjects.emplace(object);
                }
            }
            else
            {
                m_selectedObjects.clear();
                m_selectedObjects.emplace(object);
            }
        }

        //auto w = GetColumnWidth();
        //SameLine(w - 76);

        NextColumn();
        Text(object->getName().c_str());
        NextColumn();

        if (open)
        {
            for (auto child : object->getChildObjects())
            {
                showObjectTree(child);
            }
            TreePop();
        }
    }
}
