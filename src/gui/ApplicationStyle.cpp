#include "ApplicationStyle.hpp"

#include <array>
#include <bit>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iterator>

#include <pugixml.hpp>

using namespace pugi;
using namespace ImGui;


namespace
{

	static_assert(ImGuiCol_COUNT == 50);
	// If this assert fails, colors have been added/removed from ImGui, If this is the case
	// the following array need to be updated as well, and the hardcoded value in the 
	// static_assert as well.

	constexpr std::array<const char*, ImGuiCol_COUNT> imGuiColorIdStrings
	{
		"ImGuiCol_Text",
		"ImGuiCol_TextDisabled",
		"ImGuiCol_WindowBg",              // Background of normal windows
		"ImGuiCol_ChildBg",               // Background of child windows
		"ImGuiCol_PopupBg",               // Background of popups, menus, tooltips windows
		"ImGuiCol_Border",
		"ImGuiCol_BorderShadow",
		"ImGuiCol_FrameBg",               // Background of checkbox, radio button, plot, slider, text input
		"ImGuiCol_FrameBgHovered",
		"ImGuiCol_FrameBgActive",
		"ImGuiCol_TitleBg",
		"ImGuiCol_TitleBgActive",
		"ImGuiCol_TitleBgCollapsed",
		"ImGuiCol_MenuBarBg",
		"ImGuiCol_ScrollbarBg",
		"ImGuiCol_ScrollbarGrab",
		"ImGuiCol_ScrollbarGrabHovered",
		"ImGuiCol_ScrollbarGrabActive",
		"ImGuiCol_CheckMark",
		"ImGuiCol_SliderGrab",
		"ImGuiCol_SliderGrabActive",
		"ImGuiCol_Button",
		"ImGuiCol_ButtonHovered",
		"ImGuiCol_ButtonActive",
		"ImGuiCol_Header",                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		"ImGuiCol_HeaderHovered",
		"ImGuiCol_HeaderActive",
		"ImGuiCol_Separator",
		"ImGuiCol_SeparatorHovered",
		"ImGuiCol_SeparatorActive",
		"ImGuiCol_ResizeGrip",
		"ImGuiCol_ResizeGripHovered",
		"ImGuiCol_ResizeGripActive",
		"ImGuiCol_Tab",
		"ImGuiCol_TabHovered",
		"ImGuiCol_TabActive",
		"ImGuiCol_TabUnfocused",
		"ImGuiCol_TabUnfocusedActive",
		"ImGuiCol_DockingPreview",        // Preview overlay color when about to docking something
		"ImGuiCol_DockingEmptyBg",        // Background color for empty node (e.g. CentralNode with no window docked into it)
		"ImGuiCol_PlotLines",
		"ImGuiCol_PlotLinesHovered",
		"ImGuiCol_PlotHistogram",
		"ImGuiCol_PlotHistogramHovered",
		"ImGuiCol_TextSelectedBg",
		"ImGuiCol_DragDropTarget",
		"ImGuiCol_NavHighlight",          // Gamepad/keyboard: current highlighted item
		"ImGuiCol_NavWindowingHighlight", // Highlight window when using CTRL+TAB
		"ImGuiCol_NavWindowingDimBg",     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
		"ImGuiCol_ModalWindowDimBg"
	};

	const char* writeRuleCategory(StyleGroup::RuleCategory cat)
	{
		switch (cat)
		{
		case StyleGroup::RuleCategory::NodeEditorRule:
			return "NodeEditorRule";
		case StyleGroup::RuleCategory::ZigZagRule:
			return "ZigZagRule";
		default:
			return "ImGuiRule";
		}
	}

	StyleGroup::RuleCategory readRuleCategory(const char* string)
	{
		if (strcmp(string, "NodeEditorRule") == 0)
		{
			return StyleGroup::RuleCategory::NodeEditorRule;
		}
		if (strcmp(string, "ZigZagRule") == 0)
		{
			return StyleGroup::RuleCategory::ZigZagRule;
		}
		return StyleGroup::RuleCategory::ImGuiRule;
	}

	const char* writeImGuiColorId(ImGuiCol colorId)
	{
		return GetStyleColorName(colorId);
	}

	ImGuiCol readImGuiColorId(const char* string)
	{
		for (int i = 0; i < ImGuiCol_COUNT; ++i)
		{
			if (strcmp(string, imGuiColorIdStrings[i]) == 0)
			{
				return i;
			}
		}
		return 0;
	}

	std::string writeHexColor(ImVec4 color)
	{
		constexpr static std::array<char, 16> characters{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

		std::array<unsigned char, 4> bytes;
		bytes[0] = color.x * 255.0f;
		bytes[1] = color.y * 255.0f;
		bytes[2] = color.z * 255.0f;
		bytes[3] = color.w * 255.0f;

		std::string result = "#";

		for (int i = 0; i < 4; ++i)
		{
			unsigned char msbits = bytes[i] >> 4;
			unsigned char lsbits = bytes[i] & 0b00001111;
			result.push_back(characters[msbits]);
			result.push_back(characters[lsbits]);
		}
		return result;
	}

	// string doesnt need to be null terminated, but atleast 9 characters long.
	ImVec4 readHexColor(const char* string)
	{
		ImVec4 result{ 0, 0, 0, 1 };

		auto charValue = [](char character) -> int
		{ 
			if      (character >= '0' && character <= '9') return  character - '0';
			else if (character >= 'a' && character <= 'f') return (character - 'a') + 10;
			else if (character >= 'A' && character <= 'F') return (character - 'A') + 10;
			return 0;
		};

		if (string[0] == '#')
		{
			int r = charValue(string[1]) * 16 + charValue(string[2]);
			int g = charValue(string[3]) * 16 + charValue(string[4]);
			int b = charValue(string[5]) * 16 + charValue(string[6]);
			int a = charValue(string[7]) * 16 + charValue(string[8]);
			result = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		}
		return result;
	}


}


StyleGroup::StyleGroup(ApplicationStyle* appStyle, const char* name, StyleGroup* parent)
	: m_applicationStyle(appStyle),
	  m_name(name),
	  m_parent(parent)
{
	if (m_parent)
	{
		m_parent->insertChild(this);
	}
}


const std::string& StyleGroup::getName() const
{
	return m_name;
}


StyleGroup* StyleGroup::getParent()
{
	return m_parent;
}


const StyleGroup* StyleGroup::getParent() const
{
	return m_parent;
}


StyleGroup* StyleGroup::getChild(const char* name)
{
	auto it = std::lower_bound(m_children.begin(), m_children.end(), name,
		[](const auto& group, const char* name)
			{ return std::strcmp(group->getName().c_str(), name) < 0; });

	if (it != m_children.end() && (*it)->getName() == name)
	{
		return (*it);
	}
	return nullptr;
}


const StyleGroup* StyleGroup::getChild(const char* name) const
{
	auto it = std::lower_bound(m_childrenConst.begin(), m_childrenConst.end(), name,
		[](const auto& group, const char* name)
			{ return std::strcmp(group->getName().c_str(), name) < 0; });

	if (it != m_childrenConst.end() && (*it)->getName() == name)
	{
		return (*it);
	}
	return nullptr;
}


const std::vector<StyleGroup*>& StyleGroup::getChildren()
{
	return m_children;
}


const std::vector<const StyleGroup*>& StyleGroup::getChildren() const
{
	return m_childrenConst;
}


const std::vector<StyleGroup::ColorRule>& StyleGroup::getColorRules() const
{
	return m_ImGuiColorRules;
}


const std::vector<StyleGroup::SizeRule>& StyleGroup::getSizeRules() const
{
	return m_ImGuiSizeRules;
}


void StyleGroup::setColor(ImGuiCol_ colorId, ImVec4 colorValue)
{
	auto pos = getColorPos(colorId);

	if (pos != m_ImGuiColorRules.end() && pos->colorId == colorId)
	{
		*pos = { RuleCategory::ImGuiRule, colorId, colorValue, std::string(), true };
	}
	else
	{
		m_ImGuiColorRules.insert(pos, { RuleCategory::ImGuiRule, colorId, colorValue, std::string(), true });
	}
}


void StyleGroup::setColor(ImGuiCol_ colorId, const std::string& colorVariable)
{
	auto pos = getColorPos(colorId);

	if (pos != m_ImGuiColorRules.end() && pos->colorId == colorId)
	{
		*pos = { RuleCategory::ImGuiRule, colorId, {0, 0, 0, 1}, colorVariable, true };
	}
	else
	{
		m_ImGuiColorRules.insert(pos, { RuleCategory::ImGuiRule, colorId, {0, 0, 0, 1}, colorVariable, true });
	}
}


void StyleGroup::removeColor(ImGuiCol_ colorId)
{
	auto pos = getColorPos(colorId);

	if (pos != m_ImGuiColorRules.end() && pos->colorId == colorId)
	{
		m_ImGuiColorRules.erase(pos);
	}
}


void StyleGroup::setSize(ImGuiStyleVar_ sizeId, float value)
{
	auto pos = getSizePos(sizeId);

	if (pos != m_ImGuiSizeRules.end() && pos->sizeId == sizeId)
	{
		*pos = { RuleCategory::ImGuiRule, sizeId, ImVec4(value, 0, 0, 0), 1 };
	}
	else
	{
		m_ImGuiSizeRules.insert(pos, { RuleCategory::ImGuiRule, sizeId, ImVec4(value, 0, 0, 0), 1 });
	}
}


void StyleGroup::setSize(ImGuiStyleVar_ sizeId, float x, float y)
{
	auto pos = getSizePos(sizeId);

	if (pos != m_ImGuiSizeRules.end() && pos->sizeId == sizeId)
	{
		*pos = { RuleCategory::ImGuiRule, sizeId, ImVec4(x, y, 0, 0), 2 };
	}
	else
	{
		m_ImGuiSizeRules.insert(pos, { RuleCategory::ImGuiRule, sizeId, ImVec4(x, y, 0, 0), 2 });
	}
}


void StyleGroup::removeSize(ImGuiStyleVar_ sizeId)
{
	auto pos = getSizePos(sizeId);

	if (pos != m_ImGuiSizeRules.end() && pos->sizeId != sizeId)
	{
		m_ImGuiSizeRules.erase(pos);
	}
}


std::pair<ImVec4, StyleGroup::RuleSource> StyleGroup::getColorValue(ImGuiCol_ colorId) const
{
	auto rule = getColorRule(colorId);

	if (rule)
	{
		if (rule->useVariable)
		{
			return { rule->colorValue, RuleSource::Self };
		}
		else
		{
			return { m_applicationStyle->getVariableValue(rule->colorVariable), RuleSource::Self };
		}
	}
	else
	{
		if (m_parent)
		{
			auto [col, src] = m_parent->getColorValue(colorId);
			return {col, src == RuleSource::NoRule ? RuleSource::NoRule : RuleSource::Inherited };
		}
		else
		{
			return { GetStyleColorVec4(colorId), RuleSource::NoRule };
		}
	}
}


bool StyleGroup::hasColorRule(ImGuiCol_ colorId) const
{
	return getColorRule(colorId);
}


const StyleGroup::ColorRule* StyleGroup::getColorRule(ImGuiCol_ colorId) const
{
	auto it = getColorPos(colorId);

	if (it == m_ImGuiColorRules.end() || (it != m_ImGuiColorRules.end() && it->colorId != colorId))
	{
		return nullptr;
	}
	return &(*it);
}


bool StyleGroup::hasSizeRule(ImGuiStyleVar_ sizeId) const
{
	return getSizeRule(sizeId);
}


const StyleGroup::SizeRule* StyleGroup::getSizeRule(ImGuiStyleVar_ sizeId) const
{
	auto it = getSizePos(sizeId);

	if (it == m_ImGuiSizeRules.end() || (it != m_ImGuiSizeRules.end() && it->sizeId != sizeId))
	{
		return nullptr;
	}
	return &(*it);
}


void StyleGroup::insertChild(StyleGroup* child)
{
	assert(child);
	if (child)
	{
		assert(getChild(child->getName().c_str()) == nullptr);

		auto pos = std::lower_bound(m_children.begin(), m_children.end(), child->getName().c_str(),
			[](const auto& lhs, auto rhs) { return strcmp(lhs->getName().c_str(), rhs) < 0; });

		auto posConst = std::lower_bound(m_childrenConst.begin(), m_childrenConst.end(), child->getName().c_str(),
			[](const auto& lhs, auto rhs) { return strcmp(lhs->getName().c_str(), rhs) < 0; });

		m_children.insert(pos, child);
		m_childrenConst.insert(posConst, child);
	}
}


std::vector<StyleGroup::ColorRule>::iterator StyleGroup::getColorPos(ImGuiCol_ colorId)
{
	return std::lower_bound(m_ImGuiColorRules.begin(), m_ImGuiColorRules.end(),
		colorId, [](const ColorRule& lhs, ImGuiCol_ rhs) { return lhs.colorId < rhs; });
}


std::vector<StyleGroup::ColorRule>::const_iterator StyleGroup::getColorPos(ImGuiCol_ colorId) const
{
	return std::lower_bound(m_ImGuiColorRules.begin(), m_ImGuiColorRules.end(),
		colorId, [](const ColorRule& lhs, ImGuiCol_ rhs) { return lhs.colorId < rhs; });
}


std::vector<StyleGroup::SizeRule>::iterator StyleGroup::getSizePos(ImGuiStyleVar_ sizeId)
{
	return std::lower_bound(m_ImGuiSizeRules.begin(), m_ImGuiSizeRules.end(),
		sizeId, [](const SizeRule& lhs, ImGuiStyleVar_ rhs) { return lhs.sizeId < rhs; });
}


std::vector<StyleGroup::SizeRule>::const_iterator StyleGroup::getSizePos(ImGuiStyleVar_ sizeId) const
{
	return std::lower_bound(m_ImGuiSizeRules.begin(), m_ImGuiSizeRules.end(),
		sizeId, [](const SizeRule& lhs, ImGuiStyleVar_ rhs) { return lhs.sizeId < rhs; });
}


//-----------------------------------------------------------------------------


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
	xml_document document;
	auto node = document.append_child("ApplicationStyle");

	storeColorVariables(node);

	if (m_rootStyleGroup)
	{
		storeGroup(m_rootStyleGroup, node);
	}

	if (!document.save_file(fileName.c_str()))
	{
		std::cerr << "Error saving style: " << fileName << std::endl;
	}
}


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
}


void ApplicationStyle::storeColorVariables(pugi::xml_node& node) const
{
	auto colorVariablesNode = node.append_child("ColorVariables");
	for (const auto& [name, value] : m_colorVariables)
	{
		auto colorVariableNode = colorVariablesNode.append_child("ColorVariable");
		colorVariableNode.append_attribute("name").set_value(name.c_str());
		colorVariableNode.append_child(node_pcdata).set_value(writeHexColor(value).c_str());
	}
}


void ApplicationStyle::setColorVariable(const std::string& name, ImVec4 value)
{
	m_colorVariables[name] = value;
}


void ApplicationStyle::removeColorVariable(const std::string& name)
{
	/*
	 * When removing the variable we need to find all the places where it is used,
	 * and swap the use of the variable to usage of the value in the variable. only
	 * after this we can delete the variable.
	 */
	auto it = m_colorVariables.find(name);
	assert(it != m_colorVariables.end());

	if (it != m_colorVariables.end())
	{
		auto colorValue = it->second;

		for (auto& group : m_styleGroups)
		{
			// Convert all places where the variable is used to the color value:
			for (auto& colorRule : group->getColorRules())
			{
				if (!colorRule.useVariable && colorRule.colorVariable == name)
				{
					group->setColor(static_cast<ImGuiCol_>(colorRule.colorId), colorValue);
				}
			}
		}
		m_colorVariables.erase(name);
	}
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
			if (colorRule.useVariable)
			{
				ImGui::PushStyleColor(colorRule.colorId, colorRule.colorValue);
			}
			else
			{
				auto variableIt = m_colorVariables.find(colorRule.colorVariable);

				if (variableIt == m_colorVariables.end())
				{
					assert(false); // Stop in debug.
					ImGui::PushStyleColor(colorRule.colorId, 0); // in release just push 0
				}
				else
				{
					ImGui::PushStyleColor(colorRule.colorId, variableIt->second);
				}
			}
		}
		for (const auto& sizeRule : group->getSizeRules())
		{
			if (sizeRule.numDimensions == 1)
			{
				ImGui::PushStyleVar(sizeRule.sizeId, sizeRule.size.x);
			}
			else if (sizeRule.numDimensions == 2)
			{
				ImGui::PushStyleVar(sizeRule.sizeId, { sizeRule.size.x, sizeRule.size.y });
			}
		}
		int numColorRules = group->getColorRules().size();
		int numSizeRules = group->getSizeRules().size();
		m_styleGroupStack.push_back({ group, numColorRules, numSizeRules });
	}
}
