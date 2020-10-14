#include <Application.hpp>
#include <gui/widgets/TypeHierarchyWidget.hpp>
#include <object/ObjectTypeNamespace.hpp>

#include <iostream>

#include <imgui.h>


using namespace ImGui;


void TypeHierarchyWidget::draw()
{
    auto root = Application::getGlobalInstance()->getRootTypeNamespace();

    for (auto node : root->getChildren())
    {
        drawTreeNode(node);
    }
}


const ObjectType* TypeHierarchyWidget::getSelectedType() const
{
    return m_selectedType;
}


const ObjectType* TypeHierarchyWidget::getDoubleClickedType() const
{
    return m_doubleClickedType;
}


void TypeHierarchyWidget::drawTreeNode(const ObjectTypeNamespace* node)
{
	if (node)
	{
        ImGuiTreeNodeFlags flags = 0;
        flags |= ImGuiTreeNodeFlags_DefaultOpen;
        flags |= ImGuiTreeNodeFlags_SpanFullWidth;

        if (TreeNodeEx(node->getName().c_str(), flags))
        {
            for (auto leaf : node->getTypes())
            {
                drawTreeLeaf(leaf);
            }
            for (auto child : node->getChildren())
            {
                drawTreeNode(child);
            }
            TreePop();
        }
	}
}


void TypeHierarchyWidget::drawTreeLeaf(const ObjectType* leaf)
{
    if (leaf)
    {
        ImGuiTreeNodeFlags flags = 0;
        flags |= ImGuiTreeNodeFlags_SpanFullWidth;
        flags |= ImGuiTreeNodeFlags_Leaf;
        if (leaf == m_selectedType) flags |= ImGuiTreeNodeFlags_Selected;

        if (TreeNodeEx(leaf->getName().c_str(), flags))
        {
            if (IsItemClicked())
            {
                m_selectedType = leaf;
            }
            if (IsItemHovered() && IsMouseDoubleClicked(0))
            {
                m_doubleClickedType = leaf;
            }
            TreePop();
        }
    }
}
