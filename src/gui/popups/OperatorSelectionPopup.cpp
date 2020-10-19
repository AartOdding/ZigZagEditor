#include <gui/popups/OperatorSelectionPopup.hpp>

#include <imgui.h>
#include <Application.hpp>


using namespace ImGui;


void OperatorSelectionPopup::open(const ImVec2& centrePoint)
{
	OpenPopup("Operator Selection");
	m_openAt = centrePoint;
}

void OperatorSelectionPopup::update()
{
	const float e = Application::getGlobalInstance()->e();
	m_confirmedOperator = Identifier<NodeTemplate>();

	if (IsPopupOpen("Operator Selection"))
	{
		SetNextWindowSize({ 100 * e, 70 * e });
		SetNextWindowPos(m_openAt, ImGuiCond_Appearing, { 0.5, 0.5 });
	}
	if (BeginPopup("Operator Selection"))
	{
		Columns(2);

		if (m_neverOpened)
		{
			// set only once, otherwise the user cannot change the divider position
			SetColumnWidth(0, 30 * e);
			m_neverOpened = false;
		}

		BeginChild("OperatorList");
		m_operatorList.draw();
		EndChild();

		m_confirmedOperator = m_operatorList.getConfirmedOperator();

		if (m_confirmedOperator)
		{
			CloseCurrentPopup();
		}

		NextColumn();

		auto selectedOperator = m_operatorList.getSelectedOperator();
		// other things

		Columns(1);

		EndPopup();
	}
}

Identifier<NodeTemplate> OperatorSelectionPopup::getConfirmedOperator() const
{
	return m_confirmedOperator;
}
