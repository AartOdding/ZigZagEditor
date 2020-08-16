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

const std::array<const char*, 6> possibleTargets
{
	"ImGui colors",
	"NodeEditor colors",
	"ZigZag colors",
	"ImGui sizes", 
	"NodeEditor sizes",
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
	Begin(m_windowName.c_str(), p_open);
	m_appState->style.push("StyleEditorWindow");

	Columns(2);

	if (Button("Store to disk"))
	{
		m_appState->style.store("test_Style.xml");
	}
	SameLine();
	if (Button("Load from disk"))
	{
		std::cout << "loaf style from disk" << std::endl;
	}

	m_appState->style.push("StyleGroupSelector");
	drawStyleGroupTree(m_appState->style.getRootStyleGroup());
	m_appState->style.pop("StyleGroupSelector");

	// set cursor to middle 
	// draw variable editor

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

	drawColorAdjustWidget();
	drawColorListWidget();

	//EndChildFrame();


	// Text("Size rules");

	// BeginChildFrame(101, GetContentRegionAvail());

	// EndChildFrame();

	m_appState->style.pop("StyleEditorWindow");
	End();
}


void StyleEditorWindow::drawColorListWidget()
{

	if (ListBoxHeader("##ids", GetContentRegionAvail()))
	{
		if (m_selectedGroup)
		{
			for (int i = 0; i < ImGuiCol_COUNT; ++i)
			{
				PushID(i);

				auto [col, src] = m_selectedGroup->getColorValue(StyleRule::RuleTarget::ImGui, i);
				ColorEdit4("##color", &col.x, colorEditFlagsViewOnly);
				if (IsItemClicked())
				{
					m_colorIdBeingAdded = static_cast<ImGuiCol_>(i);
				}
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

}


void StyleEditorWindow::drawColorAdjustWidget()
{
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
		auto rule = m_selectedGroup->getColorRule(StyleRule::RuleTarget::ImGui, m_colorIdBeingAdded);

		if (Button(rule ? "Update" : "Set") && m_selectedGroup)
		{
			if (m_colorBeingAddedIsVariable)
			{
				m_selectedGroup->setColor(StyleRule::RuleTarget::ImGui, m_colorIdBeingAdded, m_colorBeingAddedSelectedVariable);
			}
			else
			{
				m_selectedGroup->setColor(StyleRule::RuleTarget::ImGui, m_colorIdBeingAdded, m_colorBeingAddedValue);
			}
		}
		if (rule)
		{
			SameLine();
			if (Button("Clear"))
			{
				m_selectedGroup->removeColor(StyleRule::RuleTarget::ImGui, m_colorIdBeingAdded);
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
					throw "TODO"; // cannot clear yet
					//m_appState->style.clearColorVariable(m_colorNameToRemove);
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
