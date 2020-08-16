#include "app/style/ApplicationStyle.hpp"
#include "app/style/StyleGroup.hpp"

#include <array>
#include <cassert>
#include <cstring>
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
}


bool ApplicationStyle::load(const std::string& fileName)
{
	return true;
}


void ApplicationStyle::store(const std::string& fileName) const
{
	//xml_document document;
	//auto node = document.append_child("ApplicationStyle");

	//storeColorVariables(node);

	// if (m_rootStyleGroup)
	// {
	// 	storeGroup(m_rootStyleGroup, node);
	// }

	// if (!document.save_file(fileName.c_str()))
	// {
	// 	std::cerr << "Error saving style: " << fileName << std::endl;
	// }
}

/*
void ApplicationStyle::storeGroup(StyleGroup* group, pugi::xml_node& node) const
{
	auto thisNode = node.append_child("StyleGroup");
	thisNode.append_attribute("name").set_value(group->getName().c_str());

	if (!group->getColorRules().empty())
	{
		auto colorRulesNode = thisNode.append_child("ColorRules");
		for (const auto& rule : group->getColorRules())
		{
			auto colorRuleNode = colorRulesNode.append_child("ColorRule");
			colorRuleNode.append_child("Category").append_child(node_pcdata).set_value(writeRuleCategory(rule.category));
			if (rule.category == StyleGroup::RuleCategory::ImGuiRule)
			{
				colorRuleNode.append_child("ColorId").append_child(node_pcdata).set_value(writeImGuiColorId(rule.colorId));
			}
			else if (rule.category == StyleGroup::RuleCategory::NodeEditorRule)
			{
				assert(false);
			}
			else if (rule.category == StyleGroup::RuleCategory::ZigZagRule)
			{
				assert(false);
			}
			colorRuleNode.append_child("ColorVariable").append_child(node_pcdata).set_value(rule.colorVariable.c_str());
			colorRuleNode.append_child("ColorValue").append_child(node_pcdata).set_value(writeHexColor(rule.colorValue).c_str());
			colorRuleNode.append_child("UseVariable").append_child(node_pcdata).set_value(rule.useVariable ? "true" : "false");
		}
	}
	if (!group->getChildren().empty())
	{
		auto childrenNode = thisNode.append_child("Children");
		for (auto child : group->getChildren())
		{
			storeGroup(child, childrenNode);
		}
	}
}*/


void ApplicationStyle::storeColorVariables(json& json) const
{
	// auto colorVariablesNode = node.append_child("ColorVariables");
	// for (const auto& [name, value] : m_colorVariables)
	// {
	// 	auto colorVariableNode = colorVariablesNode.append_child("ColorVariable");
	// 	colorVariableNode.append_attribute("name").set_value(name.c_str());
	// 	colorVariableNode.append_child(node_pcdata).set_value(writeHexColor(value).c_str());
	// }
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
