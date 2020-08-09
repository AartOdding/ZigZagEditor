#pragma once

#include <string>

#include "app/style/StyleEnums.hpp"



namespace StyleRule
{

    enum class RuleTarget
	{
		ImGui,
		NodeEditor,
		ZigZag
	};

    NLOHMANN_JSON_SERIALIZE_ENUM( RuleTarget, {
        { RuleTarget::ImGui, "ImGui" },
        { RuleTarget::NodeEditor, "NodeEditor" },
        { RuleTarget::ZigZag, "ZigZag" },
    })


	struct ColorRule
	{
		RuleTarget target;
		int colorId;
		ImVec4 colorValue;
		std::string colorVariable;
		bool useVariable;
	};

    void to_json(json& j, const ColorRule& p);
    void from_json(const json& j, ColorRule& p);


	struct SizeRule
	{
		RuleTarget target;
		int sizeId;
		ImVec4 size;
		int numDimensions;
	};

    void to_json(json& j, const SizeRule& p);
    void from_json(const json& j, SizeRule& p);

}
