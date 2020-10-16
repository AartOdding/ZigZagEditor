#include <Application.hpp>
#include <gui/widgets/OperatorListWidget.hpp>
#include <object/ObjectTypeNamespace.hpp>

#include <iostream>

#include <imgui.h>


using namespace ImGui;


void OperatorListWidget::draw()
{
    m_confirmedType = Identifier<ObjectType>();

    auto root = Application::getGlobalInstance()->getRootTypeNamespace();

    for (auto node : root->getChildren())
    {
        drawTreeNode(node);
    }
}


void OperatorListWidget::drawTreeNode(const ObjectTypeNamespace* node)
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


void OperatorListWidget::drawTreeLeaf(const ObjectType* leaf)
{
    if (leaf)
    {
        ImGuiTreeNodeFlags flags = 0;
        flags |= ImGuiTreeNodeFlags_SpanFullWidth;
        flags |= ImGuiTreeNodeFlags_Leaf;
        if (leaf->getIdentifier() == m_selectedType) flags |= ImGuiTreeNodeFlags_Selected;

        if (TreeNodeEx(leaf->getName().c_str(), flags))
        {
            if (IsItemClicked())
            {
                m_selectedType = leaf->getIdentifier();
            }
            if (IsItemHovered() && IsMouseDoubleClicked(0))
            {
                m_confirmedType = leaf->getIdentifier();
            }
            TreePop();
        }
    }
}


Identifier<ObjectType> OperatorListWidget::getSelectedOperator() const
{
    return m_selectedType;
}


Identifier<ObjectType> OperatorListWidget::getConfirmedOperator() const
{
    return m_confirmedType;
}