#pragma once

#include <gui/widgets/OperatorListWidget.hpp>



class OperatorSelectionPopup
{
public:

	void open();

	void draw();

	const ObjectType* getConfirmedOperator() const;

private:

	OperatorListWidget m_operatorList;

	const ObjectType* m_confirmedOperator = nullptr;

	bool m_justOpened = false;

};
