#include "app/style/HexColor.hpp"
#include "app/style/StyleRule.hpp"

using namespace ax;


namespace StyleRule
{

    void to_json(json& j, const ColorRule& rule)
    {
        j = json();
        j["target"] = rule.target;

        if (rule.target == StyleRule::RuleTarget::ImGui)
        {
            j["ruleId"] = static_cast<ImGuiCol_>(rule.ruleId);
        }
        else if (rule.target == StyleRule::RuleTarget::NodeEditor)
        {
            j["ruleId"] = static_cast<NodeEditor::StyleColor>(rule.ruleId);
        }
        else if (rule.target == StyleRule::RuleTarget::ZigZag)
        {
            // TODO: implement
            assert(false);
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
            rule.ruleId = static_cast<int>(j.at("ruleId").get<NodeEditor::StyleColor>());
        }
        else if (rule.target == StyleRule::RuleTarget::ZigZag)
        {
            // TODO: implement
            assert(false);
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
            rule.colorVariable = "";
        }
    }


    void to_json(json& j, const SizeRule& p)
    {
        // TODO: implement
        assert(false);
    }


    void from_json(const json& j, SizeRule& p)
    {
        // TODO: implement
        assert(false);
    }

}
