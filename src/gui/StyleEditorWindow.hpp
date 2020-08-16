 #pragma once

#include <array>
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

	void drawSelectedColorWidget(StyleRule::RuleTarget target, int colorCount);
	void drawColorListWidget(StyleRule::RuleTarget target, int colorCount);
	void drawColorVariableList();
	void drawStyleGroupTree(StyleGroup* group);
	float drawAddColorVariableButton();
	float drawRemoveColorVariableButton();


	std::string m_windowName;

	ApplicationState* m_appState;

	StyleGroup* m_selectedGroup = nullptr;

	// Variables for the "Add color variable" popup
	ImVec4 m_newColorVariableValue{ 0, 0, 0, 1 };
	char m_newColorVariableName[64] = "";

	// Variables for the "Remove color variable" popup
	std::string m_colorNameToRemove = "";

	// Variables for the "Add color rule" popup
	int m_colorIdBeingAdded = -1;
	std::string m_colorBeingAddedSelectedVariable = "";
	ImVec4 m_colorBeingAddedValue{ 0, 0, 0, 1 };

	std::string m_currentTarget;

};
