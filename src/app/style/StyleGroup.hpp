#pragma once

#include <memory>
#include <string>
#include <unordered_map>
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

	StyleGroup(ApplicationStyle* applicationStyle, const std::string& name);
	StyleGroup* createChild(const std::string& name);

	const std::string& getName() const;

	StyleGroup* getParent();
	const StyleGroup* getParent() const;

	StyleGroup* getChild(const std::string& name);
	const StyleGroup* getChild(const std::string& name) const;

	const std::unordered_map<std::string, std::unique_ptr<StyleGroup>>& getChildren();

	const std::vector<StyleRule::ColorRule>& getColorRules() const;
	const std::vector<StyleRule::SizeRule>& getSizeRules() const;

	int countColorRules(StyleRule::RuleTarget target) const;
	int countSizeRules(StyleRule::RuleTarget target) const;

	void setColor(StyleRule::RuleTarget target, int colorId, ImVec4 colorValue);
	void setColor(StyleRule::RuleTarget target, int colorId, const std::string& colorVariable);
	void removeColor(StyleRule::RuleTarget target, int colorId);

	void setSize(StyleRule::RuleTarget target, int sizeId, float value);
	void setSize(StyleRule::RuleTarget target, int sizeId, float x, float y);
	void removeSize(StyleRule::RuleTarget target, int sizeId);

	std::pair<ImVec4, RuleSource> getColorValue(StyleRule::RuleTarget target, int colorId) const;

	bool hasColorRule(StyleRule::RuleTarget target, int colorId) const;
	const StyleRule::ColorRule* getColorRule(StyleRule::RuleTarget target, int colorId) const;

	bool hasSizeRule(StyleRule::RuleTarget target, int sizeId) const;
	const StyleRule::SizeRule* getSizeRule(StyleRule::RuleTarget target, int sizeId) const;

private:

	StyleGroup() = delete;
	StyleGroup(const StyleGroup&) = delete;
	StyleGroup(StyleGroup&&) = delete;

	std::string m_name;

	StyleGroup* m_parent;
	std::unordered_map<std::string, std::unique_ptr<StyleGroup>> m_children;

	std::vector<StyleRule::ColorRule> m_colorRules;
	std::vector<StyleRule::SizeRule> m_sizeRules;

	ApplicationStyle* const m_applicationStyle;
    
};

