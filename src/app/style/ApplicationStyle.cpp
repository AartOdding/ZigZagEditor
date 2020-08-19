#include "app/style/ApplicationStyle.hpp"
#include "app/style/HexColor.hpp"
#include "app/style/StyleGroup.hpp"

#include <array>
#include <cassert>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>

#include <imgui_node_editor.h>

using namespace ax;
using namespace ImGui;



void ApplicationStyle::push(const std::string& groupName)
{
	if (m_styleGroupStack.empty())
	{

		if (m_rootStyleGroup)
		{
			assert(m_rootStyleGroup->getName() == groupName);
			if (m_rootStyleGroup->getName() == groupName)
			{
				pushAndApplyGroup(m_rootStyleGroup.get());
			}
		}
		else
		{
			m_rootStyleGroup = std::make_unique<StyleGroup>(this, groupName);
			pushAndApplyGroup(m_rootStyleGroup.get());
		}
	}
	else
	{
		auto group = m_styleGroupStack.back().group->getChild(groupName);

		if (group)
		{
			pushAndApplyGroup(group);
		}
		else
		{
			pushAndApplyGroup(m_styleGroupStack.back().group->createChild(groupName));
		}
	}
}


void ApplicationStyle::pop(const std::string& groupName)
{
	assert(!m_styleGroupStack.empty());
	assert(m_styleGroupStack.back().group->getName() == groupName);

	if (!m_styleGroupStack.empty() && m_styleGroupStack.back().group->getName() == groupName)
	{
		auto& frame = m_styleGroupStack.back();
		ImGui::PopStyleColor(frame.ImGuiColorPopCount);
		ImGui::PopStyleVar(frame.ImGuiSizePopCount);
		NodeEditor::PopStyleColor(frame.NodeEditorColorPopCount);
		NodeEditor::PopStyleVar(frame.NodeEditorSizePopCount);
		assert(frame.ZigZagColorPopCount == 0 && frame.ZigZagSizePopCount == 0);
		m_styleGroupStack.pop_back();
	}
	if (m_styleGroupStack.empty() && m_delayedLoadPending)
	{
		loadImplementation();
	}
}


void ApplicationStyle::setColorVariable(const std::string& name, ImVec4 value)
{
	m_colorVariables[name] = value;
}


// void ApplicationStyle::clearColorVariable(const std::string& name)
// {
// 	/*
// 	 * When removing the variable we need to find all the places where it is used,
// 	 * and swap the use of the variable to usage of the value in the variable. only
// 	 * after this we can delete the variable.
// 	 */
// 	auto it = m_colorVariables.find(name);
// 	assert(it != m_colorVariables.end());

// 	if (it != m_colorVariables.end())
// 	{
// 		auto colorValue = it->second;

// 		for (auto& group : m_styleGroups)
// 		{
// 			// Convert all places where the variable is used to the color value:
// 			for (auto& colorRule : group->getColorRules())
// 			{
// 				if (colorRule.useVariable && colorRule.colorVariable == name)
// 				{
// 					group->setColor(static_cast<ImGuiCol_>(colorRule.ruleId), colorValue);
// 				}
// 			}
// 		}
// 		m_colorVariables.erase(name);
// 	}
// }


bool ApplicationStyle::hasColorVariable(const std::string& name) const
{
	return m_colorVariables.find(name) != m_colorVariables.end();
}


ImVec4 ApplicationStyle::getVariableValue(const std::string& variableName) const
{
	auto it = m_colorVariables.find(variableName);

	if (it != m_colorVariables.end())
	{
		return it->second;
	}
	else
	{
		return { 0, 0, 0, 1 };
	}
}


const std::unordered_map<std::string, ImVec4>& ApplicationStyle::getColorVariables() const
{
	return m_colorVariables;
}


StyleGroup* ApplicationStyle::getRootStyleGroup()
{
	return m_rootStyleGroup.get();
}


const StyleGroup* ApplicationStyle::getRootStyleGroup() const
{
	return m_rootStyleGroup.get();
}


void ApplicationStyle::pushAndApplyGroup(StyleGroup* group)
{
	assert(group);
	if (group)
	{
		for (const auto& colorRule : group->getColorRules())
		{
			if (colorRule.target == StyleRule::RuleTarget::ImGui)
			{
				if (colorRule.useVariable)
				{
					ImGui::PushStyleColor(colorRule.ruleId, getVariableValue(colorRule.colorVariable));
				}
				else
				{
					ImGui::PushStyleColor(colorRule.ruleId, colorRule.colorValue);
				}
			}
			else if (colorRule.target == StyleRule::RuleTarget::NodeEditor)
			{
				auto ruleId = static_cast<NodeEditor::StyleColor>(colorRule.ruleId);
				if (colorRule.useVariable)
				{
					NodeEditor::PushStyleColor(ruleId, getVariableValue(colorRule.colorVariable));
				}
				else
				{
					NodeEditor::PushStyleColor(ruleId, colorRule.colorValue);
				}
			}
			else if (colorRule.target == StyleRule::RuleTarget::ZigZag)
			{
				// apply zigzag rule
				throw "TODO";
			}
		}
		for (const auto& sizeRule : group->getSizeRules())
		{
			if (sizeRule.target == StyleRule::RuleTarget::ImGui)
			{
				if (sizeRule.numDimensions == 1)
				{
					ImGui::PushStyleVar(sizeRule.ruleId, sizeRule.size.x);
				}
				else if (sizeRule.numDimensions == 2)
				{
					ImGui::PushStyleVar(sizeRule.ruleId, { sizeRule.size.x, sizeRule.size.y });
				}
			}
			else if (sizeRule.target == StyleRule::RuleTarget::NodeEditor)
			{
				throw "TODO";
			}
			else if (sizeRule.target == StyleRule::RuleTarget::ZigZag)
			{
				// apply zigzag rule
				throw "TODO";
			}
		}
		auto& frame = m_styleGroupStack.emplace_back();
		frame.group = group;
		frame.ImGuiColorPopCount = group->countColorRules(StyleRule::RuleTarget::ImGui);
		frame.NodeEditorColorPopCount = group->countColorRules(StyleRule::RuleTarget::NodeEditor);
		frame.ZigZagColorPopCount = group->countColorRules(StyleRule::RuleTarget::ZigZag);
		frame.ImGuiSizePopCount = group->countSizeRules(StyleRule::RuleTarget::ImGui);
		frame.NodeEditorSizePopCount = group->countSizeRules(StyleRule::RuleTarget::NodeEditor);
		frame.ZigZagSizePopCount = group->countSizeRules(StyleRule::RuleTarget::ZigZag);
	}
}


void ApplicationStyle::loadImplementation()
{
	assert(m_styleGroupStack.empty());

	if (m_styleGroupStack.empty() && m_delayedLoadPending)
	{
		std::ifstream file(m_delayedLoadFileName);
		json j;
		file >> j;

		auto colorVariables = j.at("colorVariables");
		m_colorVariables.clear();
		m_colorVariables.reserve(colorVariables.size());

		for (const auto& var : colorVariables)
		{
			m_colorVariables[var.at("name").get<std::string>()] = 
				HexColor::read(var.at("value").get<std::string>());
		}

		m_rootStyleGroup = std::make_unique<StyleGroup>(this, "");
		from_json(j.at("rootStyleGroup"), *m_rootStyleGroup);

		m_delayedLoadPending = false;
		m_delayedLoadFileName = "";

		std::cout << "loaded style from disk" << std::endl;
	}
}


void ApplicationStyle::load(const std::string& fileName)
{
	if (std::filesystem::exists(fileName))
	{
		m_delayedLoadPending = true;
		m_delayedLoadFileName = fileName;

		if (m_styleGroupStack.empty())
		{
			loadImplementation();
		}
	}
}


void ApplicationStyle::store(const std::string& fileName) const
{
	json variables = json::array();
	for (const auto& v : m_colorVariables)
	{
		variables.push_back({
			{ "name", v.first },
			{ "value", HexColor::write(v.second) }
			});
	}
	json rootStyleGroup;
	if (m_rootStyleGroup)
	{
		rootStyleGroup = *m_rootStyleGroup;
	}
	json j = { { "colorVariables", variables }, { "rootStyleGroup", rootStyleGroup } };

	std::ofstream file(fileName);
	file << std::setw(4) << j;
	std::cout << std::setw(4) << j;
}
