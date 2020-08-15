#include "app/style/HexColor.hpp"
#include "app/style/StyleRule.hpp"


namespace StyleRule
{

    void to_json(json& j, const ColorRule& rule)
    {
        j = { { "target", rule.target } };

        if (rule.target == StyleRule::RuleTarget::ImGui)
        {
            j["ruleId"] = static_cast<ImGuiCol_>(rule.ruleId);
        }
        else if (rule.target == StyleRule::RuleTarget::NodeEditor)
        {
            // TODO: implement
        }
        else if (rule.target == StyleRule::RuleTarget::ZigZag)
        {
            // TODO: implement
        }

        if (rule.useVariable)
        {
            j["variable"] = rule.colorVariable;
        }
        else
        {
            j["value"] = HexColor::write(rule.colorValue);
        }
    }


    void from_json(const json& j, ColorRule& rule)
    {
        j.at("target").get_to(rule.target);
        
        if (rule.target == StyleRule::RuleTarget::ImGui)
        {
            rule.ruleId = static_cast<int>(j.at("ruleId").get<ImGuiCol_>());
        }
        else if (rule.target == StyleRule::RuleTarget::NodeEditor)
        {
            // TODO: implement
        }
        else if (rule.target == StyleRule::RuleTarget::ZigZag)
        {
            // TODO: implement
        }

        if (j.contains("variable"))
        {
            rule.useVariable = true;
            rule.colorValue = { 0, 0, 0, 1 };
            j.at("variable").get_to(rule.colorVariable);
        }
        else
        {
            rule.useVariable = false;
            rule.colorValue = HexColor::read(j.at("value").get<std::string>());
        }
    }


    void to_json(json& j, const SizeRule& p)
    {

    }


    void from_json(const json& j, SizeRule& p)
    {
        
    }

}
