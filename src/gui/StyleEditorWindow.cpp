#include "gui/StyleEditorWindow.hpp"
#include "app/style/ApplicationStyle.hpp"
#include "app/style/StyleGroup.hpp"
#include <iostream>


using namespace ImGui;

static auto colorEditFlags =  ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_AlphaPreviewHalf;
static auto colorEditFlagsColorOnly = colorEditFlags | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar;
static auto colorEditFlagsInputsOnly = colorEditFlags | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSmallPreview;
static auto colorEditFlagsViewOnly = colorEditFlagsColorOnly | ImGuiColorEditFlags_NoPicker;

const std::array<const char*, 6> possibleTargets
{
	"ImGui colors",
	"ImGui sizes", 
	"NodeEditor colors",
	"NodeEditor sizes",
	"ZigZag colors",
	"ZigZag sizes"
};


StyleEditorWindow::StyleEditorWindow(std::string_view windowName, ApplicationState* appState)
	: m_windowName(windowName),
	  m_appState(appState)
{
	assert(m_appState);
	m_currentTarget = possibleTargets[0];
}


void StyleEditorWindow::draw(bool* p_open)
{
	m_appState->style.push("StyleEditorWindow");
	Begin(m_windowName.c_str(), p_open);

	Columns(2);

	const ImVec2 windowPadding = GetCursorPos();
	const ImVec2 contentSpace = GetContentRegionAvail(); // content space takes padding into account
	const ImVec2 itemSpacing = GetStyle().ItemSpacing;
	const float childHeight = 0.5f * (contentSpace.y - itemSpacing.y);

	// The tree widget showing the available style groups, and save / load buttons
	{
		if (Button("Save style"))
		{
			m_appState->style.store("style.json");
		}
		SameLine();
		if (Button("Load style"))
		{
			std::cout << "load style from disk" << std::endl;
		}
		float buttonHeight = GetItemRectSize().y;

		auto leftoverHeight = childHeight - buttonHeight - itemSpacing.y;
		BeginChildFrame(213, { contentSpace.x, leftoverHeight });

		m_appState->style.push("StyleGroupSelector");
		drawStyleGroupTree(m_appState->style.getRootStyleGroup());
		m_appState->style.pop("StyleGroupSelector");

		EndChildFrame();
	}

	// The color variable editor
	{
		m_appState->style.push("ColorVariablesEditor");

		auto bottomHalfStartPos = GetCursorPos();

		float buttonHeight = drawAddColorVariableButton();
		SameLine();
		buttonHeight = std::max(buttonHeight, drawRemoveColorVariableButton());

		float leftoverHeight = childHeight - buttonHeight - itemSpacing.y;
		BeginChildFrame(214, { contentSpace.x, leftoverHeight });


		drawColorVariableList();

		EndChildFrame();
		m_appState->style.pop("ColorVariablesEditor");
	}

	NextColumn();

	//Text("Color rules");

	//auto [w, h] = GetContentRegionAvail();

	//BeginChildFrame(100, { w, 0.5f * h - 10 });

	if (BeginCombo("##target", m_currentTarget.c_str()))
	{
		for (auto& t : possibleTargets)
		{
			if (Selectable(t, m_currentTarget == t))
			{
				m_currentTarget = t;
			}
		}
		EndCombo();
	}

	drawSelectedColorWidget();
	drawColorListWidget();

	//EndChildFrame();


	// Text("Size rules");

	// BeginChildFrame(101, GetContentRegionAvail());

	// EndChildFrame();

	End();
	m_appState->style.pop("StyleEditorWindow");
}


void StyleEditorWindow::drawColorListWidget()
{
	Text("Colors:");
	if (ListBoxHeader("##ids", GetContentRegionAvail()))
	{
		if (m_selectedGroup)
		{
			for (int i = 0; i < ImGuiCol_COUNT; ++i)
			{
				PushID(i);

				auto rule = m_selectedGroup->getColorRule(StyleRule::RuleTarget::ImGui, i);
				bool ruleActive = rule;
				auto [ruleColor, colorSource] = m_selectedGroup->getColorValue(StyleRule::RuleTarget::ImGui, i);

				if (Checkbox("##checkbox", &ruleActive))
				{
					if (ruleActive)
					{
						m_selectedGroup->setColor(StyleRule::RuleTarget::ImGui, i, ruleColor);
						m_colorIdBeingAdded = static_cast<ImGuiCol_>(i);
						m_colorBeingAddedValue = ruleColor;
					}
					else
					{
						m_selectedGroup->removeColor(StyleRule::RuleTarget::ImGui, i);
					}
				}
				SameLine();

				ColorEdit4("##color", &ruleColor.x, colorEditFlagsViewOnly);
				if (IsItemClicked())
				{
					m_colorIdBeingAdded = static_cast<ImGuiCol_>(i);
					m_colorBeingAddedValue = ruleColor;
				}
				SameLine();
				AlignTextToFramePadding();

				if (!ruleActive)
				{
					auto disabledCol = GetStyleColorVec4(ImGuiCol_TextDisabled);
					PushStyleColor(ImGuiCol_Text, disabledCol);
				}
				if (Selectable(ImGui::GetStyleColorName(i), m_colorIdBeingAdded == i))
				{
					m_colorIdBeingAdded = static_cast<ImGuiCol_>(i);
					m_colorBeingAddedValue = ruleColor;
				}
				if (!ruleActive)
				{
					PopStyleColor(1);
				}
				PopID();
			}
		}
		ListBoxFooter();
	}
}


void StyleEditorWindow::drawSelectedColorWidget()
{
	const StyleRule::ColorRule* colorRule = nullptr;
	bool ruleIdValid = m_colorIdBeingAdded >= 0 && m_colorIdBeingAdded < ImGuiCol_COUNT;

	if (m_selectedGroup)
	{
		colorRule = m_selectedGroup->getColorRule(StyleRule::RuleTarget::ImGui, m_colorIdBeingAdded);
	}

	if (ruleIdValid)
	{
		std::string text = "Color: ";
		text += ImGui::GetStyleColorName(m_colorIdBeingAdded);
		Text(text.c_str());
	}
	else
	{
		Text("Color: None");
	}

	bool colorExistsAndUsesValue = colorRule && !colorRule->useVariable;
	if (Checkbox("##checkBox1", &colorExistsAndUsesValue))
	{
		if (colorExistsAndUsesValue && ruleIdValid && m_selectedGroup) // checked on
		{
			m_selectedGroup->setColor(StyleRule::RuleTarget::ImGui, m_colorIdBeingAdded, m_colorBeingAddedValue);
		}
		else if (ruleIdValid && m_selectedGroup) // checked off
		{
			m_selectedGroup->removeColor(StyleRule::RuleTarget::ImGui, m_colorIdBeingAdded);
		}
	}

	SameLine();

	if (ColorEdit4("##colorValue", &m_colorBeingAddedValue.x, colorEditFlagsColorOnly))
	{
		if (ruleIdValid && m_selectedGroup)
		{
			m_selectedGroup->setColor(StyleRule::RuleTarget::ImGui, m_colorIdBeingAdded, m_colorBeingAddedValue);
		}
	}
	SameLine();
	SetNextItemWidth(GetContentRegionAvailWidth());
	if (ColorEdit4("##colorValue2", &m_colorBeingAddedValue.x, colorEditFlagsInputsOnly))
	{
		if (ruleIdValid && m_selectedGroup)
		{
			m_selectedGroup->setColor(StyleRule::RuleTarget::ImGui, m_colorIdBeingAdded, m_colorBeingAddedValue);
		}
	}

	bool colorExistsAndUsesVariable = colorRule && colorRule->useVariable;
	if (Checkbox("##checkbox2", &colorExistsAndUsesVariable))
	{
		bool varExists = m_appState->style.hasColorVariable(m_colorBeingAddedSelectedVariable);
		if (colorExistsAndUsesVariable && ruleIdValid && m_selectedGroup && varExists) // checked on
		{
			m_selectedGroup->setColor(StyleRule::RuleTarget::ImGui, m_colorIdBeingAdded, m_colorBeingAddedSelectedVariable);
		}
		else if (ruleIdValid && m_selectedGroup) // checked off
		{
			m_selectedGroup->removeColor(StyleRule::RuleTarget::ImGui, m_colorIdBeingAdded);
		}
	}

	SameLine();

	auto varColor = m_appState->style.getVariableValue(m_colorBeingAddedSelectedVariable);
	ColorEdit4("##variablePreview", &varColor.x, colorEditFlagsViewOnly);

	SameLine();

	SetNextItemWidth(GetContentRegionAvailWidth());

	if (BeginCombo("##variables", m_colorBeingAddedSelectedVariable.c_str()))
	{
		for (auto& [name, color] : m_appState->style.getColorVariables())
		{
			auto colorVec4 = color;
			PushID(&name);
			ColorEdit4("##", &colorVec4.x, colorEditFlagsViewOnly);
				
			SameLine();

			if (Selectable(name.c_str(), name == m_colorBeingAddedSelectedVariable))
			{
				m_colorBeingAddedSelectedVariable = name;
			}
			PopID();
		}
		EndCombo();
	}
}


float StyleEditorWindow::drawAddColorVariableButton()
{
	if (Button("Add color"))
	{
		OpenPopup("add color");
	}
	float height = GetItemRectSize().y;
	if (BeginPopup("add color"))
	{
		InputText("##name", m_newColorVariableName, 64);

		SameLine();
		ColorEdit4("##color", &m_newColorVariableValue.x, colorEditFlagsColorOnly);

		SameLine();
		if (Button("Add"))
		{
			if (m_newColorVariableName[0] != 0)
			{
				m_appState->style.setColorVariable(m_newColorVariableName, m_newColorVariableValue);
				m_newColorVariableName[0] = 0;
				m_newColorVariableValue = { 0, 0, 0, 1 };
			}
			CloseCurrentPopup();
		}

		EndPopup();
	}
	return height;
}


float StyleEditorWindow::drawRemoveColorVariableButton()
{
	if (Button("Remove color"))
	{
		OpenPopup("remove color");
	}
	float height = GetItemRectSize().y;
	if (BeginPopup("remove color"))
	{
		if (BeginCombo("##combo", m_colorNameToRemove.c_str()))
		{
			for (auto& [name, color] : m_appState->style.getColorVariables())
			{
				if (Selectable(name.c_str(), name == m_colorNameToRemove))
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
				throw "TODO"; // cannot clear yet
				//m_appState->style.clearColorVariable(m_colorNameToRemove);
				m_colorNameToRemove = "";
			}
			CloseCurrentPopup();
		}
		EndPopup();
	}
	return height;
}


void StyleEditorWindow::drawColorVariableList()
{
	for (auto [name, color] : m_appState->style.getColorVariables())
	{
		auto newColor = color;
		ColorEdit4(name.c_str(), &newColor.x, colorEditFlagsColorOnly);

		if (newColor.x != color.x || newColor.y != color.y || newColor.z != color.z || newColor.w != color.w)
		{
			m_appState->style.setColorVariable(name, newColor);
		}
	}
}



void StyleEditorWindow::drawStyleGroupTree(StyleGroup* group)
{
	PushID(group);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
	flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanFullWidth;
	flags |= ImGuiTreeNodeFlags_AllowItemOverlap;
	flags |= group == m_selectedGroup ? ImGuiTreeNodeFlags_Selected : 0;
	flags |= group->getChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0;
	flags |= group->getChildren().empty() ? ImGuiTreeNodeFlags_Bullet : 0;

	AlignTextToFramePadding();
	bool open = TreeNodeEx(group->getName().c_str(), flags);

	if (IsItemClicked())
	{
		m_selectedGroup = group;
	}

	if (open)
	{
		for (const auto& child : group->getChildren())
		{
			drawStyleGroupTree(child.second.get());
		}
		TreePop();
	}

	PopID();
}
