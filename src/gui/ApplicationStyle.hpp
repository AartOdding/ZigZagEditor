#pragma once

#include <cstdint>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#include <imgui.h>


class ApplicationStyle
{
public:

	void pushStyle(const std::string& groupName) const;
	void popStyle(const std::string& groupName) const;
	
	/*
	 * In all the set... functions it is important that the group to which the style option is being added,
	 * is not currently in use. This is because then a different amount of options will be popped from the
	 * imgui style stack, than that were on the stack.
	 */
	void setColor(const std::string& groupName, ImGuiCol colorId, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void setColor(const std::string& groupName, ImGuiCol colorId, const std::string& colorConstant);

	void setSize(const std::string& groupName, ImGuiStyleVar sizeId, float value);
	void setSize(const std::string& groupName, ImGuiStyleVar sizeId, float x, float y);

	void setColorConstant(const std::string& name, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void removeColorConstant(const std::string& name);

private:

	struct StyleGroup
	{
		std::vector<std::pair<int, std::uint32_t>> colors;
		std::vector<std::pair<int, std::string>> colorsFromConstants;
		std::vector<std::pair<int, float>> sizes1D;
		std::vector<std::pair<int, ImVec2>> sizes2D;
		mutable int useCount = 0;
	};

	std::unordered_map<std::string, StyleGroup> m_styleGroups;
	std::unordered_map<std::string, std::uint32_t> m_colorConstants;

};
