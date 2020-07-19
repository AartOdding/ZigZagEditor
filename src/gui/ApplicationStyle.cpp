#include "ApplicationStyle.hpp"

#include <cassert>



void ApplicationStyle::pushStyle(const std::string& groupName) const
{
	auto it = m_styleGroups.find(groupName);

	if (it != m_styleGroups.end())
	{
		for (const auto& [id, color] : it->second.colors)
		{
			ImGui::PushStyleColor(id, color);
		}
		for (const auto& [id, constant] : it->second.colorsFromConstants)
		{
			// Will throw if constant cannot be found.
			ImGui::PushStyleColor(id, m_colorConstants.at(constant));
		}
		for (const auto& [id, val] : it->second.sizes1D)
		{
			ImGui::PushStyleVar(id, val);
		}
		for (const auto& [id, val] : it->second.sizes2D)
		{
			ImGui::PushStyleVar(id, val);
		}
		it->second.useCount += 1;
	}
}


void ApplicationStyle::popStyle(const std::string& groupName) const
{
	auto it = m_styleGroups.find(groupName);

	if (it != m_styleGroups.end())
	{
		ImGui::PopStyleColor(it->second.colors.size() + it->second.colorsFromConstants.size());
		ImGui::PopStyleVar(it->second.sizes1D.size() + it->second.sizes2D.size());
		it->second.useCount -= 1;
	}
}


void ApplicationStyle::setColor(const std::string& groupName, ImGuiCol colorId, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	auto& group = m_styleGroups[groupName];
	assert(group.useCount == 0);

	if (group.useCount == 0)
	{
		group.colors.push_back({ colorId, IM_COL32(r, g, b, a) });
	}
}


void ApplicationStyle::setColor(const std::string& groupName, ImGuiCol colorId, const std::string& colorConstant)
{
	auto& group = m_styleGroups[groupName];
	assert(group.useCount == 0);
	assert(m_colorConstants.find(colorConstant) != m_colorConstants.end());

	if (group.useCount == 0 && m_colorConstants.find(colorConstant) != m_colorConstants.end())
	{
		group.colorsFromConstants.push_back({ colorId, colorConstant });
	}
}


void ApplicationStyle::setSize(const std::string& groupName, ImGuiStyleVar sizeId, float value)
{
	auto& group = m_styleGroups[groupName];
	assert(group.useCount == 0);

	if (group.useCount == 0)
	{
		group.sizes1D.push_back({ sizeId, value });
	}
}


void ApplicationStyle::setSize(const std::string& groupName, ImGuiStyleVar sizeId, float x, float y)
{
	auto& group = m_styleGroups[groupName];
	assert(group.useCount == 0);

	if (group.useCount == 0)
	{
		group.sizes2D.push_back({ sizeId, { x, y } });
	}
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
	 * In all the set... functions it is not allowed to set a style option when the 
	 * style group of that option is currently in use. In this case that is not a
	 * problem because we remove as many style options as options are added, so the 
	 * quantity stays the same.
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
					group.colors.push_back({ colorId, colorValue });
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
