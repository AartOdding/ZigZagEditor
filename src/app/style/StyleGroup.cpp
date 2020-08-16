#include "ApplicationStyle.hpp"
#include "StyleGroup.hpp"
#include "imgui_node_editor.h"

using namespace ax;
using namespace ImGui;


namespace
{
	template<typename Rule>
	typename std::vector<Rule>::iterator findPos(
		std::vector<Rule>& vec, 
		StyleRule::RuleTarget ruleTarget, 
		int ruleId)
	{
		return std::lower_bound(
			vec.begin(), 
			vec.end(), 
			std::make_pair(ruleTarget, ruleId), 
			[](const Rule& lhs, const auto& id) 
			{
				return lhs.target == id.first ? lhs.ruleId < id.second : lhs.target < id.first;
			});
	}

	template<typename Rule>
	typename std::vector<Rule>::const_iterator findPos(
		const std::vector<Rule>& vec, 
		StyleRule::RuleTarget ruleTarget, 
		int ruleId)
	{
		return std::lower_bound(
			vec.begin(),
			vec.end(), 
			std::make_pair(ruleTarget, ruleId), 
			[](const Rule& lhs, const auto& id) 
			{
				return lhs.target == id.first ? lhs.ruleId < id.second : lhs.target < id.first;
			});
	}
}


StyleGroup::StyleGroup(ApplicationStyle* appStyle, const std::string& name)
	: m_applicationStyle(appStyle),
	  m_name(name),
	  m_parent(nullptr)
{ }


StyleGroup* StyleGroup::createChild(const std::string& name)
{
	assert(m_children.find(name) == m_children.end());
	if (m_children.find(name) == m_children.end())
	{
		auto child = std::make_unique<StyleGroup>(m_applicationStyle, name);
		auto childPtr = child.get();
		child->m_parent = this;
		m_children[name] = std::move(child);
		return childPtr;
	}
	return nullptr;
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


StyleGroup* StyleGroup::getChild(const std::string& name)
{
	auto it = m_children.find(name);
	return it == m_children.end() ? nullptr : it->second.get();
}


const StyleGroup* StyleGroup::getChild(const std::string& name) const
{
	auto it = m_children.find(name);
	return it == m_children.end() ? nullptr : it->second.get();
}


const std::unordered_map<std::string, std::unique_ptr<StyleGroup>>& StyleGroup::getChildren()
{
	return m_children;
}


const std::vector<StyleRule::ColorRule>& StyleGroup::getColorRules() const
{
	return m_colorRules;
}


const std::vector<StyleRule::SizeRule>& StyleGroup::getSizeRules() const
{
	return m_sizeRules;
}


int StyleGroup::countColorRules(StyleRule::RuleTarget target) const
{
	int count = 0;
	for (const auto& c : m_colorRules)
	{
		count += static_cast<int>(c.target == target);
	}
	return count;
}


int StyleGroup::countSizeRules(StyleRule::RuleTarget target) const
{
	int count = 0;
	for (const auto& s : m_sizeRules)
	{
		count += static_cast<int>(s.target == target);
	}
	return count;
}


void StyleGroup::setColor(StyleRule::RuleTarget target, int colorId, ImVec4 colorValue)
{
	auto pos = findPos(m_colorRules, target, colorId);

	if (pos != m_colorRules.end() && pos->target == target && pos->ruleId == colorId)
	{
		*pos = { target, colorId, colorValue, std::string(), false };
	}
	else
	{
		m_colorRules.insert(pos, { target, colorId, colorValue, std::string(), false });
	}
}


void StyleGroup::setColor(StyleRule::RuleTarget target, int colorId, const std::string& colorVariable)
{
	auto pos = findPos(m_colorRules, target, colorId);

	if (pos != m_colorRules.end() && pos->target == target && pos->ruleId == colorId)
	{
		*pos = { target, colorId, {0, 0, 0, 1}, colorVariable, true };
	}
	else
	{
		m_colorRules.insert(pos, { target, colorId, {0, 0, 0, 1}, colorVariable, true });
	}
}


void StyleGroup::removeColor(StyleRule::RuleTarget target, int colorId)
{
	auto pos = findPos(m_colorRules, target, colorId);

	if (pos != m_colorRules.end() && pos->target == target && pos->ruleId == colorId)
	{
		m_colorRules.erase(pos);
	}
}


void StyleGroup::setSize(StyleRule::RuleTarget target, int sizeId, float value)
{
	auto pos = findPos(m_sizeRules, target, sizeId);

	if (pos != m_sizeRules.end() && pos->target == target && pos->ruleId == sizeId)
	{
		*pos = { target, sizeId, ImVec4(value, 0, 0, 0), 1 };
	}
	else
	{
		m_sizeRules.insert(pos, { target, sizeId, ImVec4(value, 0, 0, 0), 1 });
	}
}


void StyleGroup::setSize(StyleRule::RuleTarget target, int sizeId, float x, float y)
{
	auto pos = findPos(m_sizeRules, target, sizeId);

	if (pos != m_sizeRules.end() && pos->target == target && pos->ruleId == sizeId)
	{
		*pos = { target, sizeId, ImVec4(x, y, 0, 0), 2 };
	}
	else
	{
		m_sizeRules.insert(pos, { target, sizeId, ImVec4(x, y, 0, 0), 2 });
	}
}


void StyleGroup::removeSize(StyleRule::RuleTarget target, int sizeId)
{
	auto pos = findPos(m_sizeRules, target, sizeId);

	if (pos != m_sizeRules.end() && pos->target == target && pos->ruleId == sizeId)
	{
		m_sizeRules.erase(pos);
	}
}


std::pair<ImVec4, StyleGroup::RuleSource> StyleGroup::getColorValue(StyleRule::RuleTarget target, int colorId) const
{
	auto rule = getColorRule(target, colorId);

	if (rule)
	{
		if (rule->useVariable)
		{
			return { m_applicationStyle->getVariableValue(rule->colorVariable), RuleSource::Self };
		}
		else
		{
			return { rule->colorValue, RuleSource::Self };
		}
	}
	else
	{
		if (m_parent)
		{
			auto [col, src] = m_parent->getColorValue(target, colorId);
			return {col, src == RuleSource::NoRule ? RuleSource::NoRule : RuleSource::Inherited };
		}
		else
		{
			if (target == StyleRule::RuleTarget::ImGui)
			{
				return { GetStyleColorVec4(colorId), RuleSource::NoRule };
			}
			else if (target == StyleRule::RuleTarget::NodeEditor)
			{
				auto& style = NodeEditor::GetStyle();
				return { style.Colors[colorId], RuleSource::NoRule };
				//throw "TODO";
				//node editor way of getting a color
			}
			else if (target == StyleRule::RuleTarget::ZigZag)
			{
				throw "TODO";
				//ZigZag way of getting a color
			}
		}
	}
}


bool StyleGroup::hasColorRule(StyleRule::RuleTarget target, int colorId) const
{
	return getColorRule(target, colorId);
}


const StyleRule::ColorRule* StyleGroup::getColorRule(StyleRule::RuleTarget target, int colorId) const
{
	auto pos = findPos(m_colorRules, target, colorId);

	if (pos != m_colorRules.end() && pos->target == target && pos->ruleId == colorId)
	{
		return &(*pos);
	}
	return nullptr;
}


bool StyleGroup::hasSizeRule(StyleRule::RuleTarget target, int sizeId) const
{
	return getSizeRule(target, sizeId);
}


const StyleRule::SizeRule* StyleGroup::getSizeRule(StyleRule::RuleTarget target, int sizeId) const
{
	auto pos = findPos(m_sizeRules, target, sizeId);

	if (pos != m_sizeRules.end() && pos->target == target && pos->ruleId == sizeId)
	{
		return &(*pos);
	}
	return nullptr;
}

