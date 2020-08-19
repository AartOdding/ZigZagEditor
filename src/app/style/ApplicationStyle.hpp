#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <imgui.h>

#include "app/style/StyleGroup.hpp"


using json = nlohmann::json;


class ApplicationStyle
{
public:

	void push(const std::string& groupName);
	void pop(const std::string& groupName);

	void load(const std::string& fileName);
	void store(const std::string& fileName) const;

	void setColorVariable(const std::string& name, ImVec4 value);
	//void clearColorVariable(const std::string& name);
	bool hasColorVariable(const std::string& name) const;

	StyleGroup* getRootStyleGroup();
	const StyleGroup* getRootStyleGroup() const;

	ImVec4 getVariableValue(const std::string& variableName) const;
	const std::unordered_map<std::string, ImVec4>& getColorVariables() const;

private:

	void pushAndApplyGroup(StyleGroup* group);
	void loadImplementation();


	std::unordered_map<std::string, ImVec4> m_colorVariables;

	std::unique_ptr<StyleGroup> m_rootStyleGroup;

	// we need to store how many variables need to be popped, because we could add/ remove
	// variables from the style that are still on the imgui style stack.
	struct StyleStackFrame
	{
		StyleGroup* group = nullptr;
		int ImGuiColorPopCount = 0;
		int NodeEditorColorPopCount = 0;
		int ZigZagColorPopCount = 0;
		int ImGuiSizePopCount = 0;
		int NodeEditorSizePopCount = 0;
		int ZigZagSizePopCount = 0;
	};

	std::vector<StyleStackFrame> m_styleGroupStack;

	bool m_delayedLoadPending = false;
	std::string m_delayedLoadFileName;

};
