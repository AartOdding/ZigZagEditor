#include "ApplicationStyle.hpp"

#include <cassert>



void ApplicationStyle::push(const std::string& groupName)
{
	auto it = m_styleGroups.find(groupName);

	if (it != m_styleGroups.end())
	{
		assert(!isActive(it->second));
		assert(matchesStack(it->second));

		if (!isActive(it->second) && matchesStack(it->second))
		{
			applyStyleGroup(it->second);
			int colorPopCount = it->second.colorsFromValues.size() + it->second.colorsFromConstants.size();
			int sizePopCOunt = it->second.sizes1D.size() + it->second.sizes2D.size();
			m_styleGroupStack.push_back({ &it->second, colorPopCount, sizePopCOunt });
		}
	}
	else
	{
		m_styleGroupStack.push_back({ &createStyleGroup(groupName), 0, 0 });
	}
}


void ApplicationStyle::pop(const std::string& groupName)
{
	auto it = m_styleGroups.find(groupName);

	assert(it != m_styleGroups.end());
	assert(!m_styleGroupStack.empty());
	assert(m_styleGroupStack.back().group == &it->second);

	if (it != m_styleGroups.end() && !m_styleGroupStack.empty() && m_styleGroupStack.back().group == &it->second)
	{
		ImGui::PopStyleColor(m_styleGroupStack.back().colorPopCount);
		ImGui::PopStyleVar(m_styleGroupStack.back().sizePopCount);
		m_styleGroupStack.pop_back();
	}
}


void ApplicationStyle::setColor(const std::string& groupName, ImGuiCol colorId, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_styleGroups[groupName].colorsFromValues.push_back({ colorId, IM_COL32(r, g, b, a) });
}


void ApplicationStyle::setColor(const std::string& groupName, ImGuiCol colorId, const std::string& colorConstant)
{
	assert(m_colorConstants.find(colorConstant) != m_colorConstants.end());

	if (m_colorConstants.find(colorConstant) != m_colorConstants.end())
	{
		m_styleGroups[groupName].colorsFromConstants.push_back({ colorId, colorConstant });
	}
}


void ApplicationStyle::setSize(const std::string& groupName, ImGuiStyleVar sizeId, float value)
{
	m_styleGroups[groupName].sizes1D.push_back({ sizeId, value });
}


void ApplicationStyle::setSize(const std::string& groupName, ImGuiStyleVar sizeId, float x, float y)
{
	m_styleGroups[groupName].sizes2D.push_back({ sizeId, { x, y } });
}


void ApplicationStyle::setColorConstant(const std::string& name, ImVec4 color)
{
	m_colorConstants[name] = ImGui::ColorConvertFloat4ToU32(color);
}


void ApplicationStyle::setColorConstant(const std::string& name, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_colorConstants[name] = IM_COL32(r, g, b, a);
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

		for (auto& [groupName, group] : m_styleGroups)
		{
			// Convert all places where the constant is used to the color value:
			for (auto& [colorId, colorConstant] : group.colorsFromConstants)
			{
				if (colorConstant == name)
				{
					group.colorsFromValues.push_back({ colorId, colorValue });
				}
			}
			// Iterate backwards for safe removal.
			for (int i = group.colorsFromConstants.size() - 1; i >= 0; --i)
			{
				if (group.colorsFromConstants[i].second == name)
				{
					group.colorsFromConstants.erase(group.colorsFromConstants.begin() + i);
				}
			}
		}
		m_colorConstants.erase(name);
	}
}


const std::unordered_map<std::string, std::uint32_t>& ApplicationStyle::getColorConstants()
{
	return m_colorConstants;
}


const std::unordered_map<std::string, ApplicationStyle::StyleGroup>& ApplicationStyle::getStyleGroups()
{
	return m_styleGroups;
}


ApplicationStyle::StyleGroup& ApplicationStyle::createStyleGroup(const std::string& name)
{
	assert(m_styleGroups.find(name) == m_styleGroups.end());

	std::vector<StyleGroup*> priorStyleStack;
	priorStyleStack.reserve(m_styleGroupStack.size());

	for (auto& frame : m_styleGroupStack)
	{
		priorStyleStack.emplace_back(frame.group);
	}

	auto& newStyleGroup = m_styleGroups[name];
	newStyleGroup.priorStyleStack = std::move(priorStyleStack);
	return newStyleGroup;
}


void ApplicationStyle::applyStyleGroup(const StyleGroup& group)
{
	for (const auto& [id, color] : group.colorsFromValues)
	{
		ImGui::PushStyleColor(id, color);
	}
	for (const auto& [constantId, constantName] : group.colorsFromConstants)
	{
		auto constantIt = m_colorConstants.find(constantName);

		if (constantIt == m_colorConstants.end())
		{
			assert(false); // Stop in debug.
			ImGui::PushStyleColor(constantId, 0); // in release just push 0
		}
		else
		{
			ImGui::PushStyleColor(constantId, constantIt->second);
		}
	}
	for (const auto& [id, val] : group.sizes1D)
	{
		ImGui::PushStyleVar(id, val);
	}
	for (const auto& [id, val] : group.sizes2D)
	{
		ImGui::PushStyleVar(id, val);
	}
}


bool ApplicationStyle::isActive(const StyleGroup& group) const
{
	for (const auto& frame : m_styleGroupStack)
	{
		if (frame.group == &group)
		{
			return true;
		}
	}
	return false;
}


bool ApplicationStyle::matchesStack(const StyleGroup& group) const
{
	if (m_styleGroupStack.size() == group.priorStyleStack.size())
	{
		for (size_t i = 0; i < m_styleGroupStack.size(); ++i)
		{
			if (m_styleGroupStack.at(i).group != group.priorStyleStack.at(i))
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
