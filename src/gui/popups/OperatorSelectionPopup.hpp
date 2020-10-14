#pragma once

#include <gui/widgets/TypeHierarchyWidget.hpp>



class OperatorSelectionPopup
{
public:

	void open();

	void draw();


private:

	TypeHierarchyWidget m_operatorList;

	bool m_justOpened = false;

};
