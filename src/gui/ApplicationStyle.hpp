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

	struct StyleGroup
	{
		std::vector<StyleGroup*> priorStyleStack;
		std::vector<std::pair<int, std::uint32_t>> colorsFromValues;
		std::vector<std::pair<int, std::string>> colorsFromConstants;
		std::vector<std::pair<int, float>> sizes1D;
		std::vector<std::pair<int, ImVec2>> sizes2D;
	};

	void push(const std::string& groupName);
	void pop(const std::string& groupName);

	void setColor(const std::string& groupName, ImGuiCol colorId, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void setColor(const std::string& groupName, ImGuiCol colorId, const std::string& colorConstant);

	void setSize(const std::string& groupName, ImGuiStyleVar sizeId, float value);
	void setSize(const std::string& groupName, ImGuiStyleVar sizeId, float x, float y);

	void setColorConstant(const std::string& name, ImVec4 color);
	void setColorConstant(const std::string& name, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void removeColorConstant(const std::string& name);

	const std::unordered_map<std::string, std::uint32_t>& getColorConstants();
	const std::unordered_map<std::string, StyleGroup>&    getStyleGroups();

private:

	// we need to store how many variables need to be popped, because we could add/ remove
	// variables from the style that are still on the imgui style stack.
	struct StyleStackFrame
	{
		StyleGroup* group;
		int colorPopCount, sizePopCount;
	};

	StyleGroup& createStyleGroup(const std::string& name);
	void applyStyleGroup(const StyleGroup& group);
	bool isActive(const StyleGroup& group) const;
	bool matchesStack(const StyleGroup& group) const;

	std::unordered_map<std::string, StyleGroup> m_styleGroups;
	std::unordered_map<std::string, std::uint32_t> m_colorConstants;

	std::vector<StyleStackFrame> m_styleGroupStack;

};
