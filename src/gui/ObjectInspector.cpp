#include "ObjectInspector.hpp"

#include <imgui.h>

#include <ZigZag/Object.hpp>
#include <ZigZag/ObjectFactory.hpp>
#include <iostream>

using namespace ImGui;
using namespace ZigZag;


ObjectInspector::ObjectInspector(std::string_view windowName, ZigZag::Object* rootObject)
    : m_windowName(windowName),
      m_rootObject(rootObject),
      m_editedObject(nullptr),
      m_objectSelection(rootObject)
{
}


void ObjectInspector::setRootObject(ZigZag::Object* rootObject)
{
    m_rootObject = rootObject;
    m_objectSelection.setRootObject(rootObject);
}


void ObjectInspector::draw(bool* p_open)
{
    m_objectSelection.clearInvalidObjects();

    for (auto& [object, data] : m_objectData)
    {
        data.active = false;
    }

    Begin(m_windowName.c_str(), p_open);

    if (Button("Add Child"))
    {
        if (m_objectSelection.getSelection().size() == 1)
        {
            m_editedObject = *m_objectSelection.getSelection().begin();
            OpenPopup("Select object type");
        }
    }
    if (BeginPopup("Select object type"))
    {
        int current = -1;
        auto factory = ObjectFactory::instance();
        ListBoxHeader("##listbox");

        for (const auto& type : factory->getObjectTypeNames())
        {
            Selectable(type.c_str(), false);
            if (IsItemClicked())
            {
                auto newObject = factory->create(type);
                newObject->setParent(m_editedObject);
                newObject->setDeleteByParent(true);

                if (!GetIO().KeyShift)
                {
                    CloseCurrentPopup();
                }
            }
        }

        ListBoxFooter();

        EndPopup();
    }
    SameLine();
    if (Button("Delete"))
    {
        for (auto obj : m_objectSelection.getSelection())
        {
            delete obj;
        }
    }

    Columns(2);

    showObjectTree(m_rootObject);

    End();

    clearUnusedObjectData();
}


void ObjectInspector::showObjectTree(ZigZag::Object* object)
{
    if (object)
    {
        AlignTextToFramePadding();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
        flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
        flags |= ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanFullWidth;
        flags |= ImGuiTreeNodeFlags_AllowItemOverlap;
        flags |= m_objectSelection.isSelected(object) ? ImGuiTreeNodeFlags_Selected : 0;
        flags |= object->getChildObjects().empty() ? ImGuiTreeNodeFlags_Leaf : 0;
        flags |= object->getChildObjects().empty() ? ImGuiTreeNodeFlags_Bullet : 0;

        bool open = TreeNodeEx(object->typeName(), flags);

        if (IsItemClicked())
        {
            if (GetIO().KeyCtrl)
            {
                m_objectSelection.toggleSelected(object);
            }
            else
            {
                m_objectSelection.clearSelection();
                m_objectSelection.setSelected(object, true);
            }
        }

        //auto w = GetColumnWidth();
        //SameLine(w - 76);

        NextColumn();


        if (m_objectData.count(object) == 0)
        {
            ObjectData& data = m_objectData[object];
            data.id.reserve(object->getName().size() + 4);
            data.id = "##";
            data.id += object->getName();
            strcpy(data.name, object->getName().c_str());
        }

        ObjectData& data = m_objectData[object];
        data.active = true;
        
        PushItemWidth(-1);
        InputText(data.id.c_str(), data.name, 64);
        PopItemWidth();

        if (IsItemDeactivatedAfterEdit())
        {
            object->setName(data.name);
            if (strcmp(object->getName().c_str(), data.name) != 0)
            {
                strcpy(data.name, object->getName().c_str());
            }
            std::cout << object->getName() << "\t" << object->getFullName() << std::endl;
        }
        
        //Text(object->getName().c_str());
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


void ObjectInspector::clearUnusedObjectData()
{
    std::vector<ZigZag::Object*> toRemove;

    for (auto& [object, data] : m_objectData)
    {
        if (!data.active)
        {
            toRemove.push_back(object);
        }
    }

    for (auto object : toRemove)
    {
        m_objectData.erase(object);
    }
}
