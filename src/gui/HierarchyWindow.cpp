#include "HierarchyWindow.hpp"
#include "Application.hpp"
#include "app/ApplicationState.hpp"
#include "app/command/AddObjectCommand.hpp"
#include "app/command/RemoveObjectCommand.hpp"
#include "app/command/RenameObjectCommand.hpp"

#include <imgui.h>

#include <ZigZag/LuaBehaviour.hpp>
#include <ZigZag/ObjectFactory.hpp>
#include <iostream>

using namespace ImGui;
using namespace ZigZag;


HierarchyWindow::HierarchyWindow(std::string_view windowName, Application* app, ApplicationState* appState) : 
    Window(windowName),
    m_application(app),
    m_appState(appState)
{
}


void HierarchyWindow::setScope(ZigZag::Object* rootObject)
{
    m_rootObject = rootObject;
    m_objectSelection.setRootObject(rootObject);
}


void HierarchyWindow::draw()
{
    m_objectSelection.clearInvalidObjects();

    for (auto& [object, data] : m_objectData)
    {
        data.active = false;
    }

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

        if (ListBoxHeader("##objects"))
        {
            for (const auto& type : factory->getObjectTypeNames())
            {
                Selectable(type.c_str(), false);
                if (IsItemClicked())
                {
                    //m_appState->commandStack.pushCommand(std::make_unique<AddObjectCommand>(m_editedObject, type));
                    m_appState->commandStack.push<AddObjectCommand>(m_editedObject, type);

                    if (!GetIO().KeyShift)
                    {
                        CloseCurrentPopup();
                    }
                }
            }

            ListBoxFooter();
        }

        EndPopup();
    }

    SameLine();

    if (Button("Delete"))
    {
        for (auto obj : m_objectSelection.getSelection())
        {
            //m_appState->commandStack.pushCommand(std::make_unique<RemoveObjectCommand>(obj));
            m_appState->commandStack.push<RemoveObjectCommand>(obj);
        }
    }

    //ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg));
    //ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_Tab));
    //ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImGui::GetStyleColorVec4(ImGuiCol_TabUnfocusedActive));
    //ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyleColorVec4(ImGuiCol_TabUnfocusedActive));

    ImGui::BeginChildFrame(1343243, ImGui::GetContentRegionAvail());
    Columns(2);

    //ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);

    showObjectTree(m_rootObject);

    //ImGui::PopStyleVar();

    Columns(1);
    ImGui::EndChildFrame();
    
    //ImGui::PopStyleColor(4);

    clearUnusedObjectData();
}


void HierarchyWindow::showObjectTree(ZigZag::Object* object)
{
    if (object)
    {
        PushID(object);

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
        if (ImGui::BeginPopupContextItem())
        {
            contextMenu(object);
            ImGui::EndPopup();
        }

        NextColumn();

        bool firstOccurence = m_objectData.count(object) == 0;
        ObjectData& data = m_objectData[object];

        if (firstOccurence || strcmp(object->getName().c_str(), data.name) != 0)
        {
            strcpy(data.name, object->getName().c_str());
        }

        data.active = true;
        
        PushItemWidth(-1);
        InputText("##", data.name, 64);
        PopItemWidth();

        if (IsItemDeactivatedAfterEdit())
        {
            auto newName = object->getClosestPotentialName(data.name);

            if (object->getName() != newName)
            {
                m_appState->commandStack.push<RenameObjectCommand>(object, newName);
            }
        }
        
        NextColumn();

        PopID();

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


void HierarchyWindow::contextMenu(ZigZag::Object* object)
{
    if (ImGui::Selectable("Add Child"))
    {
        std::cout << "test" << std::endl;
    }
    if (ImGui::Selectable("Delete"))
    {
        std::cout << "test" << std::endl;
    }
    if (auto lua = dynamic_cast<ZigZag::LuaBehaviour*>(object))
    {
        ImGui::Separator();
        if (ImGui::Selectable("Edit"))
        {
            m_application->openLuaEditorWindow(lua);
        }
    }
}


void HierarchyWindow::clearUnusedObjectData()
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
