 #pragma once

#include <array>
#include <string>
#include <string_view>
#include <vector>

#include "app/ApplicationState.hpp"
#include "gui/Window.hpp"



class StyleEditorWindow : public Window
{
public:

	StyleEditorWindow(std::string_view windowName, ApplicationState* appState);

protected:

	void draw() override;

private:

	void drawSelectedColorWidget(StyleRule::RuleTarget target, int colorCount);
	void drawColorListWidget(StyleRule::RuleTarget target, int colorCount);
	void drawColorVariableList();
	void drawStyleGroupTree(StyleGroup* group);
	float drawAddColorVariableButton();
	float drawRemoveColorVariableButton();



	ApplicationState* m_appState;

	StyleGroup* m_selectedGroup = nullptr;
	int m_selectedRuleId = -1;

	// Variables for the "Add color variable" popup
	ImVec4 m_newColorVariableValue{ 0, 0, 0, 1 };
	char m_newColorVariableName[64] = "";

	// Variables for the "Remove color variable" popup
	std::string m_colorNameToRemove = "";

	// Variables for the "Add color rule" popup
	std::string m_colorBeingAddedVariable = "";
	ImVec4 m_colorBeingAddedValue{ 0, 0, 0, 1 };

	std::string m_currentTarget;

};
