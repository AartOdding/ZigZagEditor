#pragma once

#include <gui/widgets/OperatorListWidget.hpp>

#include <imgui.h>


class OperatorSelectionPopup
{
public:

	void open(const ImVec2& centrePoint);
	void update();

	Identifier<ObjectType> getConfirmedOperator() const;

private:

	OperatorListWidget m_operatorList;

	Identifier<ObjectType> m_confirmedOperator;

	bool m_neverOpened = true;
	ImVec2 m_openAt;

};
