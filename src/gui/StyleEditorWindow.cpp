#include "StyleEditorWindow.hpp"
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

	if (ListBoxHeader("##ids", { GetContentRegionAvailWidth(), 460 }))
	{
		for (int i = 0; i < ImGuiCol_COUNT; ++i)
		{
			PushID(i);
			bool colorPreviewDrawn = false;
			if (m_selectedGroup)
			{
				auto rule = m_selectedGroup->getRuleForColor(static_cast<ImGuiCol_>(i));

				if (rule)
				{
					ImVec4 col = ColorConvertU32ToFloat4(m_selectedGroup->getColorValue(rule->colorId));
					ColorEdit4("##color", &col.x, colorEditFlagsViewOnly);
					m_colorPreviewSize = GetItemRectSize();
					colorPreviewDrawn = true;
				}
			}
			if (!colorPreviewDrawn)
			{
				Dummy(m_colorPreviewSize);
			}
			SameLine();

			if (!colorPreviewDrawn)
			{
				auto disabledCol = GetStyleColorVec4(ImGuiCol_TextDisabled);
				PushStyleColor(ImGuiCol_Text, disabledCol);
			}
			if (Selectable(ImGui::GetStyleColorName(i), m_colorIdBeingAdded == i))
			{
				m_colorIdBeingAdded = static_cast<ImGuiCol_>(i);
			}
			if (!colorPreviewDrawn)
			{
				PopStyleColor(1);
			}
			PopID();
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

	auto& colors = m_appState->style.getColorConstants();
	auto it = colors.find(m_colorBeingAddedSelectedVariable);
	ImVec4 color{ 0, 0, 0, 1 };

	if (it != colors.end())
	{
		color = ColorConvertU32ToFloat4(colors.at(m_colorBeingAddedSelectedVariable));
	}

	ColorEdit4("##variablePreview", &color.x, colorEditFlagsViewOnly);
	if (IsItemClicked()) m_colorBeingAddedIsVariable = true;

	SameLine();

	// The variable picker

	SetNextItemWidth(GetContentRegionAvailWidth());

	if (BeginCombo("##constants", m_colorBeingAddedSelectedVariable.c_str()))
	{
		m_colorBeingAddedIsVariable = true;
		for (auto& [name, color] : m_appState->style.getColorConstants())
		{
			auto colorVec4 = ColorConvertU32ToFloat4(color);
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

	if (Button("Create") && m_selectedGroup)
	{
		if (m_colorBeingAddedIsVariable)
		{
			m_selectedGroup->setColor(m_colorIdBeingAdded, m_colorBeingAddedSelectedVariable);
		}
		else
		{
			m_selectedGroup->setColor(m_colorIdBeingAdded, ColorConvertFloat4ToU32(m_colorBeingAddedValue));
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
					m_appState->style.setColorConstant(m_newColorVariableName, ColorConvertFloat4ToU32(m_newColorVariableValue));
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
				for (auto& [name, color] : m_appState->style.getColorConstants())
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
			ColorEdit4(name.c_str(), &colorFloat.x, colorEditFlagsColorOnly);
			auto colorInt = ColorConvertFloat4ToU32(colorFloat);

			if (colorInt != color)
			{
				m_appState->style.setColorConstant(name, colorInt);
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
