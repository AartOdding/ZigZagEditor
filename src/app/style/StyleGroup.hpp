#pragma once

#include <string>
#include <vector>

#include <imgui.h>

#include "app/style/StyleRule.hpp"


class ApplicationStyle;

class StyleGroup
{
public:

	enum class RuleSource
	{
		Self,
		Inherited,
		NoRule
	};

	StyleGroup(ApplicationStyle* applicationStyle, const char* name, StyleGroup* parent);

	const std::string& getName() const;

	StyleGroup* getParent();
	const StyleGroup* getParent() const;

	StyleGroup* getChild(const char* name);
	const StyleGroup* getChild(const char* name) const;

	const std::vector<StyleGroup*>& getChildren();
	const std::vector<const StyleGroup*>& getChildren() const;

	const std::vector<StyleRule::ColorRule>& getColorRules() const;
	const std::vector<StyleRule::SizeRule>& getSizeRules() const;

	void setColor(StyleRule::RuleTarget target, int colorId, ImVec4 colorValue);
	void setColor(StyleRule::RuleTarget target, int colorId, const std::string& colorVariable);
	void removeColor(StyleRule::RuleTarget target, int colorId);

	void setSize(StyleRule::RuleTarget target, int sizeId, float value);
	void setSize(StyleRule::RuleTarget target, int sizeId, float x, float y);
	void removeSize(StyleRule::RuleTarget target, int sizeId);

	// TODO: change
	std::pair<ImVec4, RuleSource> getColorValue(StyleRule::RuleTarget target, int colorId) const;

	bool hasColorRule(StyleRule::RuleTarget target, int colorId) const;
	const StyleRule::ColorRule* getColorRule(StyleRule::RuleTarget target, int colorId) const;

	bool hasSizeRule(StyleRule::RuleTarget target, int sizeId) const;
	const StyleRule::SizeRule* getSizeRule(StyleRule::RuleTarget target, int sizeId) const;

private:

	StyleGroup() = delete;
	StyleGroup(const StyleGroup&) = delete;
	StyleGroup(StyleGroup&&) = delete;

	void insertChild(StyleGroup* child);

	ApplicationStyle* const m_applicationStyle;
	const std::string m_name;
	StyleGroup* const m_parent;

	std::vector<StyleGroup*> m_children;
	std::vector<const StyleGroup*> m_childrenConst;

	std::vector<StyleRule::ColorRule> m_colorRules;
	std::vector<StyleRule::SizeRule> m_sizeRules;
    
};

