#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "app/ApplicationState.hpp"



class StyleEditorWindow
{
public:

	StyleEditorWindow(std::string_view windowName, ApplicationState* appState);

	void draw(bool* p_open);

private:

	void drawColorConstantList();

	std::string m_windowName;

	ApplicationState* m_appState;

	ImVec4 m_newColorValue{ 0, 0, 0, 1 };
	char m_newColorName[64] = "";

	std::string m_colorNameToRemove = "";

	std::string m_activeColorGroup = "";

};
