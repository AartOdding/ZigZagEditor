#include "StyleEditorWindow.hpp"
#include <iostream>


using namespace ImGui;

static auto colorEditFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_Uint8;


StyleEditorWindow::StyleEditorWindow(std::string_view windowName, ApplicationState* appState)
	: m_windowName(windowName),
	  m_appState(appState)
{
	assert(m_appState);
}


void StyleEditorWindow::draw(bool* p_open)
{
	Begin(m_windowName.c_str(), p_open);
	m_appState->style.push("StyleEditorWindow");

	Columns(2);

	drawColorConstantList();

	NextColumn();

	if (BeginCombo("##groupsCombo", m_activeColorGroup.c_str()))
	{
		//for (auto& [name, group] : m_appState->style.getStyleGroups())
		//{
		//	bool selected = name == m_activeColorGroup;

		//	if (Selectable(name.c_str(), selected))
		//	{
		//		m_activeColorGroup = name;
		//	}
		//}
		EndCombo();
	}

	if (Button("Add color"))
	{

	}
	
	if (Button("Add size"))
	{

	}


	m_appState->style.pop("StyleEditorWindow");
	End();
}


void StyleEditorWindow::drawColorConstantList()
{
	if (Button("Add color"))
	{
		OpenPopup("add color");
	}
	if (BeginPopup("add color"))
	{
		InputText("##name", m_newColorName, 64);

		SameLine();
		ColorEdit4("##color", &m_newColorValue.x, colorEditFlags);

		SameLine();
		if (Button("Add"))
		{
			if (m_newColorName[0] != 0)
			{
				//m_appState->style.setColorConstant(m_newColorName, m_newColorValue);
				m_newColorName[0] = 0;
				m_newColorValue = { 0, 0, 0, 1 };
			}
			CloseCurrentPopup();
		}

		EndPopup();
	}

	SameLine();

	if (Button("Remove color"))
	{
		OpenPopup("remove color");
	}
	if (BeginPopup("remove color"))
	{
		if (BeginCombo("##combo", m_colorNameToRemove.c_str()))
		{
			for (auto& [name, color] : m_appState->style.getColorConstants())
			{
				bool selected = name == m_colorNameToRemove;
				if (Selectable(name.c_str(), selected))
				{
					m_colorNameToRemove = name;
				}
			}
			EndCombo();
		}
		SameLine();
		if (Button("Remove"))
		{
			if (m_colorNameToRemove != "")
			{
				m_appState->style.removeColorConstant(m_colorNameToRemove);
				m_colorNameToRemove = "";
			}
			CloseCurrentPopup();
		}
		EndPopup();
	}


	for (auto& [name, color] : m_appState->style.getColorConstants())
	{
		auto colorFloat = ColorConvertU32ToFloat4(color);
		ColorEdit4(name.c_str(), &colorFloat.x, colorEditFlags);
		auto colorInt = ColorConvertFloat4ToU32(colorFloat);

		if (colorInt != color)
		{
			//m_appState->style.setColorConstant(name, colorFloat);
		}
	}
}


void StyleEditorWindow::drawGroupList()
{

}