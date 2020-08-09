#pragma once

#include <string>
#include <vector>

#include <imgui.h>



class ApplicationStyle;

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

	struct ColorRule
	{
		RuleCategory category;
		int colorId;
		ImVec4 colorValue;
		std::string colorVariable;
		bool useVariable;
	};

	struct SizeRule
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

	const std::vector<ColorRule>& getColorRules() const;
	const std::vector<SizeRule>& getSizeRules() const;

	void setColor(ImGuiCol_ colorId, ImVec4 colorValue);
	void setColor(ImGuiCol_ colorId, const std::string& colorVariable);
	void removeColor(ImGuiCol_ colorId);

	void setSize(ImGuiStyleVar_ sizeId, float value);
	void setSize(ImGuiStyleVar_ sizeId, float x, float y);
	void removeSize(ImGuiStyleVar_ sizeId);

	std::pair<ImVec4, RuleSource> getColorValue(ImGuiCol_ colorId) const;

	bool hasColorRule(ImGuiCol_ colorId) const;
	const ColorRule* getColorRule(ImGuiCol_ colorId) const;

	bool hasSizeRule(ImGuiStyleVar_ sizeId) const;
	const SizeRule* getSizeRule(ImGuiStyleVar_ sizeId) const;

private:

	StyleGroup() = delete;
	StyleGroup(const StyleGroup&) = delete;
	StyleGroup(StyleGroup&&) = delete;

	void insertChild(StyleGroup* child);

	std::vector<ColorRule>::iterator getColorPos(ImGuiCol_ colorId);
	std::vector<ColorRule>::const_iterator getColorPos(ImGuiCol_ colorId) const;
	std::vector<SizeRule>::iterator getSizePos(ImGuiStyleVar_ sizeId);
	std::vector<SizeRule>::const_iterator getSizePos(ImGuiStyleVar_ sizeId) const;

	ApplicationStyle* const m_applicationStyle;
	const std::string m_name;
	StyleGroup* const m_parent;

	std::vector<StyleGroup*> m_children;
	std::vector<const StyleGroup*> m_childrenConst;

	std::vector<ColorRule> m_ImGuiColorRules;
	std::vector<SizeRule> m_ImGuiSizeRules;
};

