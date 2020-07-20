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

	void push(const std::string& groupName) const;
	void pop(const std::string& groupName) const;
	
	void setColor(const std::string& groupName, ImGuiCol colorId, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void setColor(const std::string& groupName, ImGuiCol colorId, const std::string& colorConstant);

	void setSize(const std::string& groupName, ImGuiStyleVar sizeId, float value);
	void setSize(const std::string& groupName, ImGuiStyleVar sizeId, float x, float y);

	void setColorConstant(const std::string& name, ImVec4 color);
	void setColorConstant(const std::string& name, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void removeColorConstant(const std::string& name);

	const std::unordered_map<std::string, std::uint32_t>& getColorConstants();

private:

	struct StyleGroup
	{
		std::vector<std::pair<int, std::uint32_t>> colors;
		std::vector<std::pair<int, std::string>> colorsFromConstants;
		std::vector<std::pair<int, float>> sizes1D;
		std::vector<std::pair<int, ImVec2>> sizes2D;
		mutable int colorPopCount, sizePopCount;
		mutable bool active = false;
	};

	std::unordered_map<std::string, StyleGroup> m_styleGroups;
	std::unordered_map<std::string, std::uint32_t> m_colorConstants;

};
