#include <gui/popups/OperatorSelectionPopup.hpp>

#include <imgui.h>
#include <Application.hpp>


using namespace ImGui;


void OperatorSelectionPopup::open()
{
	OpenPopup("Operator Selection");
	m_justOpened = true;
}

void OperatorSelectionPopup::draw()
{
	const float e = Application::getGlobalInstance()->e();
	m_confirmedOperator = Identifier<ObjectType>();

	if (IsPopupOpen("Operator Selection"))
	{
		SetNextWindowSize({ 100 * e, 70 * e });
	}
	if (BeginPopup("Operator Selection"))
	{
		Columns(2);

		if (m_justOpened)
		{
			// set only once, otherwise the user cannot change the divider position
			SetColumnWidth(0, 35 * e);
			m_justOpened = false;
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

Identifier<ObjectType> OperatorSelectionPopup::getConfirmedOperator() const
{
	return m_confirmedOperator;
}
