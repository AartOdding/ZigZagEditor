#pragma once

class ObjectTypeNamespace;
class ObjectType;


class TypeHierarchyWidget
{
public:

	void draw();

	const ObjectType* getSelectedType() const;
	const ObjectType* getDoubleClickedType() const;

private:

	void drawTreeNode(const ObjectTypeNamespace* node);
	void drawTreeLeaf(const ObjectType* leaf);

	const ObjectType* m_selectedType = nullptr;
	const ObjectType* m_doubleClickedType = nullptr;

};
