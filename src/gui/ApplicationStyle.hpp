#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <imgui.h>


class ApplicationStyle;


class StyleGroup
{
public:

	struct ImGuiColorRule
	{
		ImGuiCol_ colorId;
		std::uint32_t colorValue;
		std::string colorConstant;
		bool colorStoredInValue;
	};

	struct ImGuiSizeRule
	{
		ImGuiStyleVar_ sizeId;
		float size1;
		float size2;
		bool sizeIs1D;
	};

	StyleGroup(ApplicationStyle* applicationStyle, const char* name, StyleGroup* parent);

	const std::string& getName() const;

	StyleGroup* getParent();
	const StyleGroup* getParent() const;

	StyleGroup* getChild(const char* name);
	const StyleGroup* getChild(const char* name) const;

	const std::vector<StyleGroup*>& getChildren();
	const std::vector<const StyleGroup*>& getChildren() const;

	const std::vector<ImGuiColorRule>& getImGuiColorRules() const;
	const std::vector<ImGuiSizeRule>& getImGuiSizeRules() const;

	void setColor(ImGuiCol_ colorId, std::uint32_t colorValue);
	void setColor(ImGuiCol_ colorId, const std::string& colorConstant);

	void setSize(ImGuiStyleVar_ sizeId, float value);
	void setSize(ImGuiStyleVar_ sizeId, float x, float y);

	std::uint32_t getColorValue(ImGuiCol_ colorId) const;

	bool hasRuleForColor(ImGuiCol_ colorId) const;
	const ImGuiColorRule* getRuleForColor(ImGuiCol_ colorId) const;

	bool hasRuleForSize(ImGuiStyleVar_ sizeId) const;
	const ImGuiSizeRule* getRuleForSize(ImGuiStyleVar_ sizeId) const;

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

	void setColorConstant(const std::string& name, std::uint32_t value);
	void removeColorConstant(const std::string& name);

	StyleGroup* getRootStyleGroup();
	const StyleGroup* getRootStyleGroup() const;

	std::uint32_t getColorConstantValue(const std::string& constantName) const;
	const std::unordered_map<std::string, std::uint32_t>& getColorConstants() const;

private:

	StyleGroup* createGroup(const char* groupName);
	void pushAndApplyGroup(StyleGroup* group);


	std::vector<std::unique_ptr<StyleGroup>> m_styleGroups; // only needed for ownership
	std::unordered_map<std::string, std::uint32_t> m_colorConstants;

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
