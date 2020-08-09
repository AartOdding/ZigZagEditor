#pragma once

#include <imgui.h>
#include <imgui_node_editor.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// If these assertions no longer hold, the enums have to be updated.
static_assert(ImGuiCol_COUNT == 50);
static_assert(ImGuiStyleVar_COUNT == 23);


NLOHMANN_JSON_SERIALIZE_ENUM( ImGuiCol_, {
    { ImGuiCol_Text,                  "Text" },
    { ImGuiCol_TextDisabled,          "TextDisabled" },
    { ImGuiCol_WindowBg,              "WindowBg" },
    { ImGuiCol_ChildBg,               "ChildBg" },
    { ImGuiCol_PopupBg,               "PopupBg" },
    { ImGuiCol_Border,                "Border" },
    { ImGuiCol_BorderShadow,          "BorderShadow" },
    { ImGuiCol_FrameBg,               "FrameBg" },
    { ImGuiCol_FrameBgHovered,        "FrameBgHovered" },
    { ImGuiCol_FrameBgActive,         "FrameBgActive" },
    { ImGuiCol_TitleBg,               "TitleBg" },
    { ImGuiCol_TitleBgActive,         "TitleBgActive" },
    { ImGuiCol_TitleBgCollapsed,      "TitleBgCollapsed" },
    { ImGuiCol_MenuBarBg,             "MenuBarBg" },
    { ImGuiCol_ScrollbarBg,           "ScrollbarBg" },
    { ImGuiCol_ScrollbarGrab,         "ScrollbarGrab" },
    { ImGuiCol_ScrollbarGrabHovered,  "ScrollbarGrabHovered" },
    { ImGuiCol_ScrollbarGrabActive,   "ScrollbarGrabActive" },
    { ImGuiCol_CheckMark,             "CheckMark" },
    { ImGuiCol_SliderGrab,            "SliderGrab" },
    { ImGuiCol_SliderGrabActive,      "SliderGrabActive" },
    { ImGuiCol_Button,                "Button" },
    { ImGuiCol_ButtonHovered,         "ButtonHovered" },
    { ImGuiCol_ButtonActive,          "ButtonActive" },
    { ImGuiCol_Header,                "Header" },
    { ImGuiCol_HeaderHovered,         "HeaderHovered" },
    { ImGuiCol_HeaderActive,          "HeaderActive" },
    { ImGuiCol_Separator,             "Separator" },
    { ImGuiCol_SeparatorHovered,      "SeparatorHovered" },
    { ImGuiCol_SeparatorActive,       "SeparatorActive" },
    { ImGuiCol_ResizeGrip,            "ResizeGrip" },
    { ImGuiCol_ResizeGripHovered,     "ResizeGripHovered" },
    { ImGuiCol_ResizeGripActive,      "ResizeGripActive" },
    { ImGuiCol_Tab,                   "Tab" },
    { ImGuiCol_TabHovered,            "TabHovered" },
    { ImGuiCol_TabActive,             "TabActive" },
    { ImGuiCol_TabUnfocused,          "TabUnfocused" },
    { ImGuiCol_TabUnfocusedActive,    "TabUnfocusedActive" },
    { ImGuiCol_DockingPreview,        "DockingPreview" },
    { ImGuiCol_DockingEmptyBg,        "DockingEmptyBg" },
    { ImGuiCol_PlotLines,             "PlotLines" },
    { ImGuiCol_PlotLinesHovered,      "PlotLinesHovered" },
    { ImGuiCol_PlotHistogram,         "PlotHistogram" },
    { ImGuiCol_PlotHistogramHovered,  "PlotHistogramHovered" },
    { ImGuiCol_TextSelectedBg,        "TextSelectedBg" },
    { ImGuiCol_DragDropTarget,        "DragDropTarget" },
    { ImGuiCol_NavHighlight,          "NavHighlight" },
    { ImGuiCol_NavWindowingHighlight, "NavWindowingHighlight" },
    { ImGuiCol_NavWindowingDimBg,     "NavWindowingDimBg" },
    { ImGuiCol_ModalWindowDimBg,      "ModalWindowDimBg" }
})


NLOHMANN_JSON_SERIALIZE_ENUM( ImGuiStyleVar_, {
    { ImGuiStyleVar_Alpha,               "Alpha" },
    { ImGuiStyleVar_WindowPadding,       "WindowPadding" },
    { ImGuiStyleVar_WindowRounding,      "WindowRounding" },
    { ImGuiStyleVar_WindowBorderSize,    "WindowBorderSize" },
    { ImGuiStyleVar_WindowMinSize,       "WindowMinSize" },
    { ImGuiStyleVar_WindowTitleAlign,    "WindowTitleAlign" },
    { ImGuiStyleVar_ChildRounding,       "ChildRounding" },
    { ImGuiStyleVar_ChildBorderSize,     "ChildBorderSize" },
    { ImGuiStyleVar_PopupRounding,       "PopupRounding" },
    { ImGuiStyleVar_PopupBorderSize,     "PopupBorderSize" },
    { ImGuiStyleVar_FramePadding,        "FramePadding" },
    { ImGuiStyleVar_FrameRounding,       "FrameRounding" },
    { ImGuiStyleVar_FrameBorderSize,     "FrameBorderSize" },
    { ImGuiStyleVar_ItemSpacing,         "ItemSpacing" },
    { ImGuiStyleVar_ItemInnerSpacing,    "ItemInnerSpacing" },
    { ImGuiStyleVar_IndentSpacing,       "IndentSpacing" },
    { ImGuiStyleVar_ScrollbarSize,       "ScrollbarSize" },
    { ImGuiStyleVar_ScrollbarRounding,   "ScrollbarRounding" },
    { ImGuiStyleVar_GrabMinSize,         "GrabMinSize" },
    { ImGuiStyleVar_GrabRounding,        "GrabRounding" },
    { ImGuiStyleVar_TabRounding,         "TabRounding" },
    { ImGuiStyleVar_ButtonTextAlign,     "ButtonTextAlign" },
    { ImGuiStyleVar_SelectableTextAlign, "SelectableTextAlign" }
})

