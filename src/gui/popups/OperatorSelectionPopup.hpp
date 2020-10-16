#pragma once

#include <gui/widgets/OperatorListWidget.hpp>



class OperatorSelectionPopup
{
public:

	void open();
	void draw();

	Identifier<ObjectType> getConfirmedOperator() const;

private:

	OperatorListWidget m_operatorList;

	Identifier<ObjectType> m_confirmedOperator;

	bool m_justOpened = false;

};
