#pragma once

#include <object/ObjectType.hpp>

class ObjectTypeNamespace;


class OperatorListWidget
{
public:

	void draw();

	// If the list does not get drawn, the hovered, selected and confirmed
	// operators don't get updated either!
	Identifier<ObjectType> getHoveredOperator() const;
	Identifier<ObjectType> getSelectedOperator() const;
	Identifier<ObjectType> getConfirmedOperator() const;

private:

	void drawTreeNode(const ObjectTypeNamespace* node);
	void drawTreeLeaf(const ObjectType* leaf);

	Identifier<ObjectType> m_hoveredType;
	Identifier<ObjectType> m_selectedType;
	Identifier<ObjectType> m_confirmedType;

};
