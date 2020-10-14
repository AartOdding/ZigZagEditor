#include <gui/popups/OperatorSelectionPopup.hpp>

#include <imgui.h>

using namespace ImGui;


void OperatorSelectionPopup::open()
{
	OpenPopup("Operator Selection");
	m_justOpened = true;
}

void OperatorSelectionPopup::draw()
{
	if (IsPopupOpen("Operator Selection"))
	{
		SetNextWindowSize({ 600, 400 });
	}
	if (BeginPopup("Operator Selection"))
	{
		Columns(2);

		if (m_justOpened)
		{
			SetColumnWidth(0, 200);
			SetColumnWidth(1, 400);
			m_justOpened = false;
		}

		m_operatorList.draw();
		NextColumn();

		// other things

		Columns(1);

		EndPopup();
	}
}

