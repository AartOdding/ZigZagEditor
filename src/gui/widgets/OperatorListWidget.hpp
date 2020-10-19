#pragma once

#include <object/Template.hpp>

class TemplateGroup;


class OperatorListWidget
{
public:

	void draw();

	// If the list does not get drawn, the hovered, selected and confirmed
	// operators don't get updated either!
	Identifier<Template> getHoveredOperator() const;
	Identifier<Template> getSelectedOperator() const;
	Identifier<Template> getConfirmedOperator() const;

private:

	void drawTreeNode(const TemplateGroup* node);
	void drawTreeLeaf(const Template* leaf);

	Identifier<Template> m_hoveredType;
	Identifier<Template> m_selectedType;
	Identifier<Template> m_confirmedType;

};
