#include "gui/StyleEditorWindow.hpp"
#include "app/style/ApplicationStyle.hpp"
#include "app/style/StyleGroup.hpp"
#include <iostream>


using namespace ImGui;

static auto colorEditFlags =  ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_AlphaPreviewHalf;
static auto colorEditFlagsColorOnly = colorEditFlags | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar;
static auto colorEditFlagsInputsOnly = colorEditFlags | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSmallPreview;
static auto colorEditFlagsViewOnly = colorEditFlagsColorOnly | ImGuiColorEditFlags_NoPicker;

ImVec2 StyleEditorWindow::m_colorPreviewSize{ 10, 10 };


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

	if (Button("Edit color variables"))
	{
		OpenPopup("ColorVariablesEditor");
	}
	drawColorVariablesEditorPopup();

	if (Button("Store to disk"))
	{
		m_appState->style.store("test_Style.xml");
	}


	m_appState->style.push("StyleGroupSelector");
	drawGroupListItem(m_appState->style.getRootStyleGroup());
	m_appState->style.pop("StyleGroupSelector");


	Text("Color rules");

	auto [w, h] = GetContentRegionAvail();

	BeginChildFrame(100, { w, 0.5f * h - 10 });

	drawAddColorRulePopup();

	EndChildFrame();


	Text("Size rules");

	BeginChildFrame(101, GetContentRegionAvail());

	EndChildFrame();

	m_appState->style.pop("StyleEditorWindow");
	End();
}


void StyleEditorWindow::drawAddColorRulePopup()
{
	Columns(2);

	if (ListBoxHeader("##ids", GetContentRegionAvail()))
	{
		if (m_selectedGroup)
		{
			for (int i = 0; i < ImGuiCol_COUNT; ++i)
			{
				PushID(i);

				auto [col, src] = m_selectedGroup->getColorValue(static_cast<ImGuiCol_>(i));
				ColorEdit4("##color", &col.x, colorEditFlagsViewOnly);
				SameLine();
				auto dummySize = GetItemRectSize();
				auto cursorPos = GetCursorPos();
				AlignTextToFramePadding();
				if (src == StyleGroup::RuleSource::Inherited) Text(">>");
				SetCursorPos(cursorPos);
				Dummy(dummySize);
				SameLine();
				AlignTextToFramePadding();

				if (src != StyleGroup::RuleSource::Self)
				{
					auto disabledCol = GetStyleColorVec4(ImGuiCol_TextDisabled);
					PushStyleColor(ImGuiCol_Text, disabledCol);
				}
				if (Selectable(ImGui::GetStyleColorName(i), m_colorIdBeingAdded == i))
				{
					m_colorIdBeingAdded = static_cast<ImGuiCol_>(i);
				}
				if (src != StyleGroup::RuleSource::Self)
				{
					PopStyleColor(1);
				}
				PopID();
			}
		}
		ListBoxFooter();
	}

	NextColumn();

	// The name of the active Id.
	Text(ImGui::GetStyleColorName(m_colorIdBeingAdded));


	// Start on new line for the direct setting of a color with a value:

	bool colorBeingAddedIsValue = !m_colorBeingAddedIsVariable;
	if (Checkbox("##value", &colorBeingAddedIsValue))
	{
		m_colorBeingAddedIsVariable = false;
	}

	SameLine();

	ColorEdit4("##colorValue", &m_colorBeingAddedValue.x, colorEditFlagsColorOnly);
	if (IsItemClicked()) m_colorBeingAddedIsVariable = false;

	SameLine();
	SetNextItemWidth(GetContentRegionAvailWidth());
	ColorEdit4("##colorValue2", &m_colorBeingAddedValue.x, colorEditFlagsInputsOnly);
	if (IsItemClicked()) m_colorBeingAddedIsVariable = false;



	// The row to create a color rule from a variable.
	// First the check button:
	if (Checkbox("##variable", &m_colorBeingAddedIsVariable))
	{
		m_colorBeingAddedIsVariable = true;
	}

	SameLine();

	// The variable color preview

	auto& colors = m_appState->style.getColorVariables();
	auto it = colors.find(m_colorBeingAddedSelectedVariable);
	ImVec4 color{ 0, 0, 0, 1 };

	if (it != colors.end())
	{
		color = it->second;
	}

	ColorEdit4("##variablePreview", &color.x, colorEditFlagsViewOnly);
	if (IsItemClicked()) m_colorBeingAddedIsVariable = true;

	SameLine();

	// The variable picker

	SetNextItemWidth(GetContentRegionAvailWidth());

	if (BeginCombo("##variables", m_colorBeingAddedSelectedVariable.c_str()))
	{
		m_colorBeingAddedIsVariable = true;
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

	if (m_selectedGroup)
	{
		auto rule = m_selectedGroup->getColorRule(m_colorIdBeingAdded);
		const char* text = rule ? "Update" : "Create";

		if (Button(rule ? "Update" : "Create") && m_selectedGroup)
		{
			if (m_colorBeingAddedIsVariable)
			{
				m_selectedGroup->setColor(m_colorIdBeingAdded, m_colorBeingAddedSelectedVariable);
			}
			else
			{
				m_selectedGroup->setColor(m_colorIdBeingAdded, m_colorBeingAddedValue);
			}
		}
		if (rule)
		{
			SameLine();
			if (Button("Delete"))
			{
				m_selectedGroup->removeColor(m_colorIdBeingAdded);
			}
		}
	}



}


void StyleEditorWindow::drawColorVariablesEditorPopup()
{
	if (BeginPopup("ColorVariablesEditor"))
	{
		m_appState->style.push("ColorVariablesEditor");

		if (Button("Add color"))
		{
			OpenPopup("add color");
		}
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

		SameLine();

		if (Button("Remove color"))
		{
			OpenPopup("remove color");
		}
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
					m_appState->style.clearColorVariable(m_colorNameToRemove);
					m_colorNameToRemove = "";
				}
				CloseCurrentPopup();
			}
			EndPopup();
		}

		for (auto [name, color] : m_appState->style.getColorVariables())
		{
			auto newColor = color;
			ColorEdit4(name.c_str(), &newColor.x, colorEditFlagsColorOnly);

			if (newColor.x != color.x || newColor.y != color.y || newColor.z != color.z || newColor.w != color.w)
			{
				m_appState->style.setColorVariable(name, newColor);
			}
		}

		m_appState->style.pop("ColorVariablesEditor");
		EndPopup();
	}
}



void StyleEditorWindow::drawGroupListItem(StyleGroup* group)
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
		for (auto child : group->getChildren())
		{
			drawGroupListItem(child);
		}
		TreePop();
	}

	PopID();
}
