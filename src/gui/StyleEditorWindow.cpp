#include "gui/StyleEditorWindow.hpp"
#include "app/style/ApplicationStyle.hpp"
#include "app/style/StyleGroup.hpp"

#include <iostream>

#include <imgui_node_editor.h>

using namespace ax;


namespace
{
	auto colorEditFlags = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_AlphaPreviewHalf;
	auto colorEditFlagsColorOnly = colorEditFlags | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar;
	auto colorEditFlagsInputsOnly = colorEditFlags | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSmallPreview;
	auto colorEditFlagsViewOnly = colorEditFlagsColorOnly | ImGuiColorEditFlags_NoPicker;

	const std::array<const char*, 6> possibleTargets
	{
		"ImGui colors",
		"ImGui sizes",
		"NodeEditor colors",
		"NodeEditor sizes",
		"ZigZag colors",
		"ZigZag sizes"
	};

	const char* getColorName(StyleRule::RuleTarget target, int colorId)
	{
		if (target == StyleRule::RuleTarget::ImGui)
		{
			return ImGui::GetStyleColorName(colorId);
		}
		else if (target == StyleRule::RuleTarget::NodeEditor)
		{
			return NodeEditor::GetStyleColorName(static_cast<NodeEditor::StyleColor>(colorId));
		}
		return "Error";
	}
}



StyleEditorWindow::StyleEditorWindow(std::string_view windowName, ApplicationState* appState)
	: Window(windowName),
	  m_appState(appState)
{
	assert(m_appState);
	m_currentTarget = possibleTargets[0];
}


void StyleEditorWindow::draw()
{
	m_appState->style.push("StyleEditorWindow");

	ImGui::Columns(2);

	const ImVec2 windowPadding = ImGui::GetCursorPos();
	const ImVec2 contentSpace = ImGui::GetContentRegionAvail(); // content space takes padding into account
	const ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
	const float childHeight = 0.5f * (contentSpace.y - itemSpacing.y);

	// The tree widget showing the available style groups, and save / load buttons
	{
		if (ImGui::Button("Save style"))
		{
			m_appState->style.store("style.json");
		}
		ImGui::SameLine();
		if (ImGui::Button("Load style"))
		{
			m_appState->style.load("style.json");
		}
		float buttonHeight = ImGui::GetItemRectSize().y;

		auto leftoverHeight = childHeight - buttonHeight - itemSpacing.y;
		ImGui::BeginChildFrame(213, { contentSpace.x, leftoverHeight });

		m_appState->style.push("StyleGroupSelector");
		drawStyleGroupTree(m_appState->style.getRootStyleGroup());
		m_appState->style.pop("StyleGroupSelector");

		ImGui::EndChildFrame();
	}

	// The color variable editor
	{
		m_appState->style.push("ColorVariablesEditor");

		auto bottomHalfStartPos = ImGui::GetCursorPos();

		float buttonHeight = drawAddColorVariableButton();
		ImGui::SameLine();
		buttonHeight = std::max(buttonHeight, drawRemoveColorVariableButton());

		float leftoverHeight = childHeight - buttonHeight - itemSpacing.y;
		ImGui::BeginChildFrame(214, { contentSpace.x, leftoverHeight });


		drawColorVariableList();

		m_appState->style.pop("ColorVariablesEditor");
		ImGui::EndChildFrame();
	}

	ImGui::NextColumn();

	if (ImGui::BeginCombo("##target", m_currentTarget.c_str()))
	{
		for (auto& t : possibleTargets)
		{
			if (ImGui::Selectable(t, m_currentTarget == t))
			{
				m_currentTarget = t;
			}
		}
		ImGui::EndCombo();
	}

	if (m_currentTarget == possibleTargets[0])
	{
		drawSelectedColorWidget(StyleRule::RuleTarget::ImGui, ImGuiCol_COUNT);
		drawColorListWidget(StyleRule::RuleTarget::ImGui, ImGuiCol_COUNT);
	}
	else if (m_currentTarget == possibleTargets[2])
	{
		drawSelectedColorWidget(StyleRule::RuleTarget::NodeEditor, NodeEditor::StyleColor_Count);
		drawColorListWidget(StyleRule::RuleTarget::NodeEditor, NodeEditor::StyleColor_Count);
	}

	m_appState->style.pop("StyleEditorWindow");
}


void StyleEditorWindow::drawColorListWidget(StyleRule::RuleTarget target, int colorCount)
{
	ImGui::Text("Colors:");
	if (ImGui::ListBoxHeader("##ids", ImGui::GetContentRegionAvail()))
	{
		if (m_selectedGroup)
		{
			for (int i = 0; i < colorCount; ++i)
			{
				ImGui::PushID(i);

				auto rule = m_selectedGroup->getColorRule(target, i);
				bool ruleActive = rule;
				auto [ruleColor, colorSource] = m_selectedGroup->getColorValue(target, i);

				if (ImGui::Checkbox("##checkbox", &ruleActive))
				{
					if (ruleActive)
					{
						m_selectedGroup->setColor(target, i, ruleColor);
						m_selectedRuleId = i;
						m_colorBeingAddedValue = ruleColor;
						m_colorBeingAddedVariable = rule ? rule->colorVariable : "";
					}
					else
					{
						m_selectedGroup->removeColor(target, i);
					}
				}
				ImGui::SameLine();

				ImGui::ColorEdit4("##color", &ruleColor.x, colorEditFlagsViewOnly);
				if (ImGui::IsItemClicked())
				{
					m_selectedRuleId = i;
					m_colorBeingAddedValue = ruleColor;
					m_colorBeingAddedVariable = rule ? rule->colorVariable : "";
				}
				ImGui::SameLine();
				ImGui::AlignTextToFramePadding();

				if (!ruleActive)
				{
					auto disabledCol = ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
					ImGui::PushStyleColor(ImGuiCol_Text, disabledCol);
				}
				if (ImGui::Selectable(getColorName(target, i), m_selectedRuleId == i))
				{
					m_selectedRuleId = i;
					m_colorBeingAddedValue = ruleColor;
					m_colorBeingAddedVariable = rule ? rule->colorVariable : "";
				}
				if (!ruleActive)
				{
					ImGui::PopStyleColor(1);
				}
				ImGui::PopID();
			}
		}
		ImGui::ListBoxFooter();
	}
}


void StyleEditorWindow::drawSelectedColorWidget(StyleRule::RuleTarget target, int colorCount)
{
	const StyleRule::ColorRule* colorRule = nullptr;
	bool ruleIdValid = m_selectedRuleId >= 0 && m_selectedRuleId < colorCount;

	if (m_selectedGroup)
	{
		colorRule = m_selectedGroup->getColorRule(target, m_selectedRuleId);
	}

	if (ruleIdValid)
	{
		std::string text = "Color: ";
		text += getColorName(target, m_selectedRuleId);
		ImGui::Text(text.c_str());
	}
	else
	{
		ImGui::Text("Color: None");
	}

	bool colorExistsAndUsesValue = colorRule && !colorRule->useVariable;
	if (ImGui::Checkbox("##checkBox1", &colorExistsAndUsesValue))
	{
		if (colorExistsAndUsesValue && ruleIdValid && m_selectedGroup) // checked on
		{
			m_selectedGroup->setColor(target, m_selectedRuleId, m_colorBeingAddedValue);
		}
		else if (ruleIdValid && m_selectedGroup) // checked off
		{
			m_selectedGroup->removeColor(target, m_selectedRuleId);
		}
	}

	ImGui::SameLine();

	if (ImGui::ColorEdit4("##colorValue", &m_colorBeingAddedValue.x, colorEditFlagsColorOnly))
	{
		if (ruleIdValid && m_selectedGroup)
		{
			m_selectedGroup->setColor(target, m_selectedRuleId, m_colorBeingAddedValue);
		}
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	if (ImGui::ColorEdit4("##colorValue2", &m_colorBeingAddedValue.x, colorEditFlagsInputsOnly))
	{
		if (ruleIdValid && m_selectedGroup)
		{
			m_selectedGroup->setColor(target, m_selectedRuleId, m_colorBeingAddedValue);
		}
	}

	bool colorExistsAndUsesVariable = colorRule && colorRule->useVariable;
	if (ImGui::Checkbox("##checkbox2", &colorExistsAndUsesVariable))
	{
		bool varExists = m_appState->style.hasColorVariable(m_colorBeingAddedVariable);
		if (colorExistsAndUsesVariable && ruleIdValid && m_selectedGroup && varExists) // checked on
		{
			m_selectedGroup->setColor(target, m_selectedRuleId, m_colorBeingAddedVariable);
		}
		else if (ruleIdValid && m_selectedGroup) // checked off
		{
			m_selectedGroup->removeColor(target, m_selectedRuleId);
		}
	}

	ImGui::SameLine();

	auto varColor = m_appState->style.getVariableValue(m_colorBeingAddedVariable);
	ImGui::ColorEdit4("##variablePreview", &varColor.x, colorEditFlagsViewOnly);

	ImGui::SameLine();

	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());

	if (ImGui::BeginCombo("##variables", m_colorBeingAddedVariable.c_str()))
	{
		for (auto& [name, color] : m_appState->style.getColorVariables())
		{
			auto colorVec4 = color;
			ImGui::PushID(&name);
			ImGui::ColorEdit4("##", &colorVec4.x, colorEditFlagsViewOnly);
			if (ImGui::IsItemClicked())
			{
				ImGui::CloseCurrentPopup();
				m_colorBeingAddedVariable = name;
				if (m_selectedGroup) m_selectedGroup->setColor(target, m_selectedRuleId, name);
			}
			ImGui::SameLine();

			if (ImGui::Selectable(name.c_str(), name == m_colorBeingAddedVariable))
			{
				m_colorBeingAddedVariable = name;
				if (m_selectedGroup) m_selectedGroup->setColor(target, m_selectedRuleId, name);
			}
			ImGui::PopID();
		}
		ImGui::EndCombo();
	}
}


float StyleEditorWindow::drawAddColorVariableButton()
{
	if (ImGui::Button("Add color"))
	{
		ImGui::OpenPopup("add color");
	}
	float height = ImGui::GetItemRectSize().y;
	if (ImGui::BeginPopup("add color"))
	{
		ImGui::InputText("##name", m_newColorVariableName, 64);

		ImGui::SameLine();
		ImGui::ColorEdit4("##color", &m_newColorVariableValue.x, colorEditFlagsColorOnly);

		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			if (m_newColorVariableName[0] != 0)
			{
				m_appState->style.setColorVariable(m_newColorVariableName, m_newColorVariableValue);
				m_newColorVariableName[0] = 0;
				m_newColorVariableValue = { 0, 0, 0, 1 };
			}
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	return height;
}


float StyleEditorWindow::drawRemoveColorVariableButton()
{
	if (ImGui::Button("Remove color"))
	{
		ImGui::OpenPopup("remove color");
	}
	float height = ImGui::GetItemRectSize().y;
	if (ImGui::BeginPopup("remove color"))
	{
		if (ImGui::BeginCombo("##combo", m_colorNameToRemove.c_str()))
		{
			for (auto& [name, color] : m_appState->style.getColorVariables())
			{
				if (ImGui::Selectable(name.c_str(), name == m_colorNameToRemove))
				{
					m_colorNameToRemove = name;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove"))
		{
			if (m_colorNameToRemove != "")
			{
				throw "TODO"; // cannot clear yet
				//m_appState->style.clearColorVariable(m_colorNameToRemove);
				m_colorNameToRemove = "";
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return height;
}


void StyleEditorWindow::drawColorVariableList()
{
	for (auto [name, color] : m_appState->style.getColorVariables())
	{
		auto newColor = color;
		ImGui::ColorEdit4(name.c_str(), &newColor.x, colorEditFlagsColorOnly);

		if (newColor.x != color.x || newColor.y != color.y || newColor.z != color.z || newColor.w != color.w)
		{
			m_appState->style.setColorVariable(name, newColor);
		}
	}
}


void StyleEditorWindow::drawStyleGroupTree(StyleGroup* group)
{
	ImGui::PushID(group);

	int ruleCount = 0;
	if      (m_currentTarget == possibleTargets[0]) ruleCount = group->countColorRules(StyleRule::RuleTarget::ImGui);
	else if (m_currentTarget == possibleTargets[1]) ruleCount = group->countSizeRules(StyleRule::RuleTarget::ImGui);
	else if (m_currentTarget == possibleTargets[2]) ruleCount = group->countColorRules(StyleRule::RuleTarget::NodeEditor);
	else if (m_currentTarget == possibleTargets[3]) ruleCount = group->countSizeRules(StyleRule::RuleTarget::NodeEditor);
	else if (m_currentTarget == possibleTargets[4]) ruleCount = group->countColorRules(StyleRule::RuleTarget::ZigZag);
	else if (m_currentTarget == possibleTargets[5]) ruleCount = group->countSizeRules(StyleRule::RuleTarget::ZigZag);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
	flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanFullWidth;
	flags |= ImGuiTreeNodeFlags_AllowItemOverlap;
	flags |= group == m_selectedGroup ? ImGuiTreeNodeFlags_Selected : 0;
	flags |= group->getChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0;
	flags |= group->getChildren().empty() ? ImGuiTreeNodeFlags_Bullet : 0;

	bool hasRule = false;
	if      (m_currentTarget == possibleTargets[0]) hasRule = group->hasColorRule(StyleRule::RuleTarget::ImGui, m_selectedRuleId);
	else if (m_currentTarget == possibleTargets[1]) hasRule = group->hasSizeRule(StyleRule::RuleTarget::ImGui, m_selectedRuleId);
	else if (m_currentTarget == possibleTargets[2]) hasRule = group->hasColorRule(StyleRule::RuleTarget::NodeEditor, m_selectedRuleId);
	else if (m_currentTarget == possibleTargets[3]) hasRule = group->hasSizeRule(StyleRule::RuleTarget::NodeEditor, m_selectedRuleId);
	else if (m_currentTarget == possibleTargets[4]) hasRule = group->hasColorRule(StyleRule::RuleTarget::ZigZag, m_selectedRuleId);
	else if (m_currentTarget == possibleTargets[5]) hasRule = group->hasSizeRule(StyleRule::RuleTarget::ZigZag, m_selectedRuleId);

	if (!hasRule)
	{
		auto disabledCol = ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
		ImGui::PushStyleColor(ImGuiCol_Text, disabledCol);
	}
	ImGui::AlignTextToFramePadding();
	auto text = group->getName() + " (" + std::to_string(ruleCount) + ")";
	bool open = ImGui::TreeNodeEx(text.c_str(), flags);

	if (ImGui::IsItemClicked())
	{
		m_selectedGroup = group;
	}
	if (!hasRule)
	{
		ImGui::PopStyleColor(1);
	}

	if (open)
	{
		for (auto& child : group->getChildren())
		{
			drawStyleGroupTree(&child.second);
		}
		ImGui::TreePop();
	}

	ImGui::PopID();
}
