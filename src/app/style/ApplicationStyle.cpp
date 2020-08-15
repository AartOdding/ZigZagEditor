#include "app/style/ApplicationStyle.hpp"
#include "app/style/StyleGroup.hpp"

#include <array>
#include <bit>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iterator>


using namespace ImGui;



void ApplicationStyle::push(const char* groupName)
{
	if (m_styleGroupStack.empty())
	{
		if (m_rootStyleGroup)
		{
			assert(m_rootStyleGroup->getName() == groupName);
			if (m_rootStyleGroup->getName() == groupName)
			{
				pushAndApplyGroup(m_rootStyleGroup);
			}
		}
		else
		{
			m_rootStyleGroup = createGroup(groupName);
			pushAndApplyGroup(m_rootStyleGroup);
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
			auto newGroup = createGroup(groupName);
			pushAndApplyGroup(newGroup);
		}
	}
}


void ApplicationStyle::pop(const char* groupName)
{
	assert(!m_styleGroupStack.empty());
	assert(m_styleGroupStack.back().group->getName() == groupName);

	if (!m_styleGroupStack.empty() && m_styleGroupStack.back().group->getName() == groupName)
	{
		ImGui::PopStyleColor(m_styleGroupStack.back().colorPopCount);
		ImGui::PopStyleVar(m_styleGroupStack.back().sizePopCount);
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
	return m_rootStyleGroup;
}


const StyleGroup* ApplicationStyle::getRootStyleGroup() const
{
	return m_rootStyleGroup;
}


StyleGroup* ApplicationStyle::createGroup(const char* groupName)
{
	if (m_styleGroupStack.empty())
	{
		assert(!m_rootStyleGroup);
		m_styleGroups.push_back(std::make_unique<StyleGroup>(this, groupName, nullptr));
	}
	else
	{
		m_styleGroups.push_back(std::make_unique<StyleGroup>(this, groupName, m_styleGroupStack.back().group));
	}
	return m_styleGroups.back().get();
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
					auto variableIt = m_colorVariables.find(colorRule.colorVariable);

					if (variableIt == m_colorVariables.end())
					{
						assert(false); // Stop in debug.
						ImGui::PushStyleColor(colorRule.ruleId, ColorConvertFloat4ToU32({0, 0, 0, 1})); // in release just push black
					}
					else
					{
						ImGui::PushStyleColor(colorRule.ruleId, variableIt->second);
					}
				}
				else
				{
					ImGui::PushStyleColor(colorRule.ruleId, colorRule.colorValue);
				}
			}
			else if (colorRule.target == StyleRule::RuleTarget::NodeEditor)
			{
				// apply node editor rule
				throw "TODO";
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
		int numColorRules = group->getColorRules().size();
		int numSizeRules = group->getSizeRules().size();
		m_styleGroupStack.push_back({ group, numColorRules, numSizeRules });
	}
}
