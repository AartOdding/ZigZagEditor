#pragma once

class ObjectTypeNamespace;
class ObjectType;


class OperatorListWidget
{
public:

	void draw();

	const ObjectType* getSelectedOperator() const;
	const ObjectType* getConfirmedOperator() const;

private:

	void drawTreeNode(const ObjectTypeNamespace* node);
	void drawTreeLeaf(const ObjectType* leaf);

	const ObjectType* m_selectedType = nullptr;
	const ObjectType* m_doubleClickedType = nullptr;

};
