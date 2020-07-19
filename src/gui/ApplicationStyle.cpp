#include "ApplicationStyle.hpp"

#include <cassert>



void ApplicationStyle::push(const std::string& groupName) const
{
	auto it = m_styleGroups.find(groupName);

	if (it != m_styleGroups.end() && !it->second.active)
	{
		// we need to store how many variables need to be popped, because we could add/ remove
		// variables from the style that are still on the imgui style stack.
		it->second.colorPopCount = it->second.colors.size() + it->second.colorsFromConstants.size();
		it->second.sizePopCount = it->second.sizes1D.size() + it->second.sizes2D.size();
		it->second.active = true;

		for (const auto& [id, color] : it->second.colors)
		{
			ImGui::PushStyleColor(id, color);
		}
		for (const auto& [id, constant] : it->second.colorsFromConstants)
		{
			auto constantIt = m_colorConstants.find(constant);

			if (constantIt == m_colorConstants.end())
			{
				assert(false); // Stop in debug.
				--it->second.colorPopCount; // Pop one less because we cant push.
			}
			else
			{
				ImGui::PushStyleColor(id, constantIt->second);
			}
		}
		for (const auto& [id, val] : it->second.sizes1D)
		{
			ImGui::PushStyleVar(id, val);
		}
		for (const auto& [id, val] : it->second.sizes2D)
		{
			ImGui::PushStyleVar(id, val);
		}
	}
}


void ApplicationStyle::pop(const std::string& groupName) const
{
	auto it = m_styleGroups.find(groupName);

	if (it != m_styleGroups.end())
	{
		ImGui::PopStyleColor(it->second.colorPopCount);
		ImGui::PopStyleVar(it->second.sizes1D.size() + it->second.sizes2D.size());
		it->second.active = false;
	}
}


void ApplicationStyle::setColor(const std::string& groupName, ImGuiCol colorId, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_styleGroups[groupName].colors.push_back({ colorId, IM_COL32(r, g, b, a) });
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
