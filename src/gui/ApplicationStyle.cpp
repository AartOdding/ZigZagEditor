#include "ApplicationStyle.hpp"

#include <cassert>
#include <cstring>
#include <iterator>

using namespace ImGui;


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


const std::vector<StyleGroup::ImGuiColorRule>& StyleGroup::getImGuiColorRules() const
{
	return m_ImGuiColorRules;
}


const std::vector<StyleGroup::ImGuiSizeRule>& StyleGroup::getImGuiSizeRules() const
{
	return m_ImGuiSizeRules;
}


void StyleGroup::setColor(ImGuiCol_ colorId, std::uint32_t colorValue)
{
	auto pos = getColorPos(colorId);

	if (pos == m_ImGuiColorRules.end() || (pos != m_ImGuiColorRules.end() && pos->colorId != colorId))
	{
		// if there is no rule for given colorId yet
		m_ImGuiColorRules.insert(pos, { colorId, colorValue, std::string(), true });
	}
	else
	{
		*pos = { colorId, colorValue, std::string(), true };
	}
}


void StyleGroup::setColor(ImGuiCol_ colorId, const std::string& colorConstant)
{
	auto pos = getColorPos(colorId);

	if (pos == m_ImGuiColorRules.end() || (pos != m_ImGuiColorRules.end() && pos->colorId != colorId))
	{
		// if there is no rule for given colorId yet
		m_ImGuiColorRules.insert(pos, { colorId, 0, colorConstant, true });
	}
	else
	{
		*pos = { colorId, 0, colorConstant, true };
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

	if (pos == m_ImGuiSizeRules.end() || (pos != m_ImGuiSizeRules.end() && pos->sizeId != sizeId))
	{
		// if there is no rule for given colorId yet
		m_ImGuiSizeRules.insert(pos, { sizeId, value, 0.0f, true });
	}
	else
	{
		*pos = { sizeId, value, 0.0f, true };
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


void StyleGroup::setSize(ImGuiStyleVar_ sizeId, float x, float y)
{
	auto pos = getSizePos(sizeId);

	if (pos == m_ImGuiSizeRules.end() || (pos != m_ImGuiSizeRules.end() && pos->sizeId != sizeId))
	{
		// if there is no rule for given colorId yet
		m_ImGuiSizeRules.insert(pos, { sizeId, x, y, false });
	}
	else
	{
		*pos = { sizeId, x, y, false };
	}
}


std::pair<std::uint32_t, StyleGroup::ValueSource> StyleGroup::getColorValue(ImGuiCol_ colorId) const
{
	auto rule = getRuleForColor(colorId);

	if (rule)
	{
		if (rule->colorStoredInValue)
		{
			return { rule->colorValue, ValueSource::Self };
		}
		else
		{
			return { m_applicationStyle->getColorConstantValue(rule->colorConstant), ValueSource::Self };
		}
	}
	else
	{
		if (m_parent)
		{
			auto [col, src] = m_parent->getColorValue(colorId);
			return {col, src == ValueSource::NoRule ? ValueSource::NoRule : ValueSource::Inherited };
		}
		else
		{
			return { ColorConvertFloat4ToU32(GetStyleColorVec4(colorId)), ValueSource::NoRule };
		}
	}
}


bool StyleGroup::hasRuleForColor(ImGuiCol_ colorId) const
{
	return getRuleForColor(colorId);
}


const StyleGroup::ImGuiColorRule* StyleGroup::getRuleForColor(ImGuiCol_ colorId) const
{
	auto it = getColorPos(colorId);

	if (it == m_ImGuiColorRules.end() || (it != m_ImGuiColorRules.end() && it->colorId != colorId))
	{
		return nullptr;
	}
	return &(*it);
}


bool StyleGroup::hasRuleForSize(ImGuiStyleVar_ sizeId) const
{
	return getRuleForSize(sizeId);
}


const StyleGroup::ImGuiSizeRule* StyleGroup::getRuleForSize(ImGuiStyleVar_ sizeId) const
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


std::vector<StyleGroup::ImGuiColorRule>::iterator StyleGroup::getColorPos(ImGuiCol_ colorId)
{
	return std::lower_bound(m_ImGuiColorRules.begin(), m_ImGuiColorRules.end(),
		colorId, [](const ImGuiColorRule& lhs, ImGuiCol_ rhs) { return lhs.colorId < rhs; });
}


std::vector<StyleGroup::ImGuiColorRule>::const_iterator StyleGroup::getColorPos(ImGuiCol_ colorId) const
{
	return std::lower_bound(m_ImGuiColorRules.begin(), m_ImGuiColorRules.end(),
		colorId, [](const ImGuiColorRule& lhs, ImGuiCol_ rhs) { return lhs.colorId < rhs; });
}


std::vector<StyleGroup::ImGuiSizeRule>::iterator StyleGroup::getSizePos(ImGuiStyleVar_ sizeId)
{
	return std::lower_bound(m_ImGuiSizeRules.begin(), m_ImGuiSizeRules.end(),
		sizeId, [](const ImGuiSizeRule& lhs, ImGuiStyleVar_ rhs) { return lhs.sizeId < rhs; });
}


std::vector<StyleGroup::ImGuiSizeRule>::const_iterator StyleGroup::getSizePos(ImGuiStyleVar_ sizeId) const
{
	return std::lower_bound(m_ImGuiSizeRules.begin(), m_ImGuiSizeRules.end(),
		sizeId, [](const ImGuiSizeRule& lhs, ImGuiStyleVar_ rhs) { return lhs.sizeId < rhs; });
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


void ApplicationStyle::setColorConstant(const std::string& name, std::uint32_t value)
{
	m_colorConstants[name] = value;
}


void ApplicationStyle::removeColorConstant(const std::string& name)
{
	/*
	 * When removing the constant we need to find all the places where it is used,
	 * and swap the use of the constant to usage of the value in the constant. only
	 * after this we can delete the constant.
	 */
	auto it = m_colorConstants.find(name);
	assert(it != m_colorConstants.end());

	if (it != m_colorConstants.end())
	{
		auto colorValue = it->second;

		for (auto& group : m_styleGroups)
		{
			// Convert all places where the constant is used to the color value:
			for (auto& colorRule : group->getImGuiColorRules())
			{
				if (!colorRule.colorStoredInValue && colorRule.colorConstant == name)
				{
					group->setColor(colorRule.colorId, colorValue);
				}
			}
		}
		m_colorConstants.erase(name);
	}
}


std::uint32_t ApplicationStyle::getColorConstantValue(const std::string& constantName) const
{
	auto it = m_colorConstants.find(constantName);

	if (it != m_colorConstants.end())
	{
		return it->second;
	}
	else
	{
		return ImGui::ColorConvertFloat4ToU32({ 0, 0, 0, 1 });
	}
}


const std::unordered_map<std::string, std::uint32_t>& ApplicationStyle::getColorConstants() const
{
	return m_colorConstants;
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
		for (const auto& colorRule : group->getImGuiColorRules())
		{
			if (colorRule.colorStoredInValue)
			{
				ImGui::PushStyleColor(colorRule.colorId, colorRule.colorValue);
			}
			else
			{
				auto constantIt = m_colorConstants.find(colorRule.colorConstant);

				if (constantIt == m_colorConstants.end())
				{
					assert(false); // Stop in debug.
					ImGui::PushStyleColor(colorRule.colorId, 0); // in release just push 0
				}
				else
				{
					ImGui::PushStyleColor(colorRule.colorId, constantIt->second);
				}
			}
		}
		for (const auto& sizeRule : group->getImGuiSizeRules())
		{
			if (sizeRule.sizeIs1D)
			{
				ImGui::PushStyleVar(sizeRule.sizeId, sizeRule.size1);
			}
			else
			{
				ImGui::PushStyleVar(sizeRule.sizeId, { sizeRule.size1, sizeRule.size2 });
			}
		}
		int numColorRules = group->getImGuiColorRules().size();
		int numSizeRules = group->getImGuiSizeRules().size();
		m_styleGroupStack.push_back({ group, numColorRules, numSizeRules });
	}
}
