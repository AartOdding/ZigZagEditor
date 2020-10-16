#pragma once

#include <object/ObjectType.hpp>

class ObjectTypeNamespace;


class OperatorListWidget
{
public:

	void draw();

	Identifier<ObjectType> getSelectedOperator() const;
	Identifier<ObjectType> getConfirmedOperator() const;

private:

	void drawTreeNode(const ObjectTypeNamespace* node);
	void drawTreeLeaf(const ObjectType* leaf);

	Identifier<ObjectType> m_selectedType;
	Identifier<ObjectType> m_confirmedType;

};
