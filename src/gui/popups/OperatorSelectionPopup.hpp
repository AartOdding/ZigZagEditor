#pragma once

#include <gui/widgets/OperatorListWidget.hpp>

#include <imgui.h>


class OperatorSelectionPopup
{
public:

	void open(const ImVec2& centrePoint);
	void update();

	Identifier<Template> getConfirmedOperator() const;

private:

	OperatorListWidget m_operatorList;

	Identifier<Template> m_confirmedOperator;

	bool m_neverOpened = true;
	ImVec2 m_openAt;

};
