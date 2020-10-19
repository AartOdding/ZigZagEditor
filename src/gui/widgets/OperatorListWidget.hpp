#pragma once

#include <object/NodeTemplate.hpp>

class TemplateGroup;


class OperatorListWidget
{
public:

	void draw();

	// If the list does not get drawn, the hovered, selected and confirmed
	// operators don't get updated either!
	Identifier<NodeTemplate> getHoveredOperator() const;
	Identifier<NodeTemplate> getSelectedOperator() const;
	Identifier<NodeTemplate> getConfirmedOperator() const;

private:

	void drawTreeNode(const TemplateGroup* node);
	void drawTreeLeaf(const NodeTemplate* leaf);

	Identifier<NodeTemplate> m_hoveredType;
	Identifier<NodeTemplate> m_selectedType;
	Identifier<NodeTemplate> m_confirmedType;

};
