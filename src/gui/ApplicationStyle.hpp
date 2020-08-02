#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <imgui.h>


class ApplicationStyle;
namespace pugi
{
	class xml_node;
}


class StyleGroup
{
public:

	enum class RuleCategory
	{
		ImGuiRule,
		NodeEditorRule,
		ZigZagRule
	};

	enum class RuleSource
	{
		Self,
		Inherited,
		NoRule
	};

	struct ImGuiColorRule
	{
		RuleCategory category;
		int colorId;
		std::uint32_t colorValue;
		std::string colorVariable;
		bool useVariable;
	};

	struct ImGuiSizeRule
	{
		RuleCategory category;
		int sizeId;
		ImVec4 size;
		int numDimensions;
	};

	StyleGroup(ApplicationStyle* applicationStyle, const char* name, StyleGroup* parent);

	const std::string& getName() const;

	StyleGroup* getParent();
	const StyleGroup* getParent() const;

	StyleGroup* getChild(const char* name);
	const StyleGroup* getChild(const char* name) const;

	const std::vector<StyleGroup*>& getChildren();
	const std::vector<const StyleGroup*>& getChildren() const;

	const std::vector<ImGuiColorRule>& getColorRules() const;
	const std::vector<ImGuiSizeRule>& getSizeRules() const;

	void setColor(ImGuiCol_ colorId, std::uint32_t colorValue);
	void setColor(ImGuiCol_ colorId, const std::string& colorVariable);
	void removeColor(ImGuiCol_ colorId);

	void setSize(ImGuiStyleVar_ sizeId, float value);
	void setSize(ImGuiStyleVar_ sizeId, float x, float y);
	void removeSize(ImGuiStyleVar_ sizeId);

	std::pair<std::uint32_t, RuleSource> getColorValue(ImGuiCol_ colorId) const;

	bool hasColorRule(ImGuiCol_ colorId) const;
	const ImGuiColorRule* getColorRule(ImGuiCol_ colorId) const;

	bool hasSizeRule(ImGuiStyleVar_ sizeId) const;
	const ImGuiSizeRule* getSizeRule(ImGuiStyleVar_ sizeId) const;

private:

	StyleGroup() = delete;
	StyleGroup(const StyleGroup&) = delete;
	StyleGroup(StyleGroup&&) = delete;

	void insertChild(StyleGroup* child);

	std::vector<ImGuiColorRule>::iterator getColorPos(ImGuiCol_ colorId);
	std::vector<ImGuiColorRule>::const_iterator getColorPos(ImGuiCol_ colorId) const;
	std::vector<ImGuiSizeRule>::iterator getSizePos(ImGuiStyleVar_ sizeId);
	std::vector<ImGuiSizeRule>::const_iterator getSizePos(ImGuiStyleVar_ sizeId) const;

	ApplicationStyle* const m_applicationStyle;
	const std::string m_name;
	StyleGroup* const m_parent;

	std::vector<StyleGroup*> m_children;
	std::vector<const StyleGroup*> m_childrenConst;

	std::vector<ImGuiColorRule> m_ImGuiColorRules;
	std::vector<ImGuiSizeRule> m_ImGuiSizeRules;
};



class ApplicationStyle
{
public:

	void push(const char* groupName);
	void pop(const char* groupName);

	bool load(const std::string& fileName);
	void store(const std::string& fileName) const;

	void setColorVariable(const std::string& name, std::uint32_t value);
	void removeColorVariable(const std::string& name);

	StyleGroup* getRootStyleGroup();
	const StyleGroup* getRootStyleGroup() const;

	std::uint32_t getVariableValue(const std::string& variableName) const;
	const std::unordered_map<std::string, std::uint32_t>& getColorVariables() const;

private:

	StyleGroup* createGroup(const char* groupName);
	void pushAndApplyGroup(StyleGroup* group);
	void storeGroup(StyleGroup* group, pugi::xml_node& node) const;
	void storeColorVariables(pugi::xml_node& node) const;


	std::vector<std::unique_ptr<StyleGroup>> m_styleGroups; // only needed for ownership
	std::unordered_map<std::string, std::uint32_t> m_colorVariables;

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
