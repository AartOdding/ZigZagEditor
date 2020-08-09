#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <imgui.h>

#include "app/style/StyleGroup.hpp"


class ApplicationStyle;
namespace pugi
{
	class xml_node;
}

class StyleGroup;

class ApplicationStyle
{
public:

	void push(const char* groupName);
	void pop(const char* groupName);

	bool load(const std::string& fileName);
	void store(const std::string& fileName) const;

	void setColorVariable(const std::string& name, ImVec4 value);
	void removeColorVariable(const std::string& name);

	StyleGroup* getRootStyleGroup();
	const StyleGroup* getRootStyleGroup() const;

	ImVec4 getVariableValue(const std::string& variableName) const;
	const std::unordered_map<std::string, ImVec4>& getColorVariables() const;

private:

	StyleGroup* createGroup(const char* groupName);
	void pushAndApplyGroup(StyleGroup* group);
	void storeGroup(StyleGroup* group, pugi::xml_node& node) const;
	void storeColorVariables(pugi::xml_node& node) const;


	std::vector<std::unique_ptr<StyleGroup>> m_styleGroups; // only needed for ownership
	std::unordered_map<std::string, ImVec4> m_colorVariables;

	StyleGroup* m_rootStyleGroup = nullptr;

	// we need to store how many variables need to be popped, because we could add/ remove
	// variables from the style that are still on the imgui style stack.
	struct StyleStackFrame
	{
		StyleGroup* group;
		int colorPopCount, sizePopCount;
	};

	std::vector<StyleStackFrame> m_styleGroupStack;

};
