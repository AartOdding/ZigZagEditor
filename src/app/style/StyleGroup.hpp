#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "app/style/StyleRule.hpp"


using json = nlohmann::json;

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

	StyleGroup() = default;
	StyleGroup(const StyleGroup&) = delete;
	StyleGroup(StyleGroup&&) = default;

	StyleGroup(ApplicationStyle* applicationStyle, const std::string& name);
	StyleGroup* createChild(const std::string& name);

	const std::string& getName() const;

	StyleGroup* getParent();
	const StyleGroup* getParent() const;

	StyleGroup* getChild(const std::string& name);
	const StyleGroup* getChild(const std::string& name) const;

	std::unordered_map<std::string, StyleGroup>& getChildren();
	const std::unordered_map<std::string, StyleGroup>& getChildren() const;

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

	friend void to_json(json& j, const StyleGroup& sg);
	friend void from_json(const json& j, StyleGroup& sg);

	std::string m_name;

	StyleGroup* m_parent;
	std::unordered_map<std::string, StyleGroup> m_children;

	std::vector<StyleRule::ColorRule> m_colorRules;
	std::vector<StyleRule::SizeRule> m_sizeRules;

	ApplicationStyle* m_applicationStyle;
    
};


void to_json(json& j, const StyleGroup& sg);
void from_json(const json& j, StyleGroup& sg);
