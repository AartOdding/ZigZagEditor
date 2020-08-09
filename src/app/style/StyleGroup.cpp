#include "ApplicationStyle.hpp"
#include "StyleGroup.hpp"


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


const std::vector<StyleRule::ColorRule>& StyleGroup::getColorRules() const
{
	return m_ImGuiColorRules;
}


const std::vector<StyleRule::SizeRule>& StyleGroup::getSizeRules() const
{
	return m_ImGuiSizeRules;
}


void StyleGroup::setColor(ImGuiCol_ colorId, ImVec4 colorValue)
{
	auto pos = getColorPos(colorId);

	if (pos != m_ImGuiColorRules.end() && pos->colorId == colorId)
	{
		*pos = { StyleRule::RuleTarget::ImGui, colorId, colorValue, std::string(), false };
	}
	else
	{
		m_ImGuiColorRules.insert(pos, { StyleRule::RuleTarget::ImGui, colorId, colorValue, std::string(), false });
	}
}


void StyleGroup::setColor(ImGuiCol_ colorId, const std::string& colorVariable)
{
	auto pos = getColorPos(colorId);

	if (pos != m_ImGuiColorRules.end() && pos->colorId == colorId)
	{
		*pos = { StyleRule::RuleTarget::ImGui, colorId, {0, 0, 0, 1}, colorVariable, true };
	}
	else
	{
		m_ImGuiColorRules.insert(pos, { StyleRule::RuleTarget::ImGui, colorId, {0, 0, 0, 1}, colorVariable, true });
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
		*pos = { StyleRule::RuleTarget::ImGui, sizeId, ImVec4(value, 0, 0, 0), 1 };
	}
	else
	{
		m_ImGuiSizeRules.insert(pos, { StyleRule::RuleTarget::ImGui, sizeId, ImVec4(value, 0, 0, 0), 1 });
	}
}


void StyleGroup::setSize(ImGuiStyleVar_ sizeId, float x, float y)
{
	auto pos = getSizePos(sizeId);

	if (pos != m_ImGuiSizeRules.end() && pos->sizeId == sizeId)
	{
		*pos = { StyleRule::RuleTarget::ImGui, sizeId, ImVec4(x, y, 0, 0), 2 };
	}
	else
	{
		m_ImGuiSizeRules.insert(pos, { StyleRule::RuleTarget::ImGui, sizeId, ImVec4(x, y, 0, 0), 2 });
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


const StyleRule::ColorRule* StyleGroup::getColorRule(ImGuiCol_ colorId) const
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


const StyleRule::SizeRule* StyleGroup::getSizeRule(ImGuiStyleVar_ sizeId) const
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


std::vector<StyleRule::ColorRule>::iterator StyleGroup::getColorPos(ImGuiCol_ colorId)
{
	return std::lower_bound(m_ImGuiColorRules.begin(), m_ImGuiColorRules.end(),
		colorId, [](const StyleRule::ColorRule& lhs, ImGuiCol_ rhs) { return lhs.colorId < rhs; });
}


std::vector<StyleRule::ColorRule>::const_iterator StyleGroup::getColorPos(ImGuiCol_ colorId) const
{
	return std::lower_bound(m_ImGuiColorRules.begin(), m_ImGuiColorRules.end(),
		colorId, [](const StyleRule::ColorRule& lhs, ImGuiCol_ rhs) { return lhs.colorId < rhs; });
}


std::vector<StyleRule::SizeRule>::iterator StyleGroup::getSizePos(ImGuiStyleVar_ sizeId)
{
	return std::lower_bound(m_ImGuiSizeRules.begin(), m_ImGuiSizeRules.end(),
		sizeId, [](const StyleRule::SizeRule& lhs, ImGuiStyleVar_ rhs) { return lhs.sizeId < rhs; });
}


std::vector<StyleRule::SizeRule>::const_iterator StyleGroup::getSizePos(ImGuiStyleVar_ sizeId) const
{
	return std::lower_bound(m_ImGuiSizeRules.begin(), m_ImGuiSizeRules.end(),
		sizeId, [](const StyleRule::SizeRule& lhs, ImGuiStyleVar_ rhs) { return lhs.sizeId < rhs; });
}

