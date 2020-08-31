#pragma once

#include "gui/Window.hpp"
#include <TextEditor.h>


class LuaEditorWindow : public Window
{
public:

	LuaEditorWindow(std::string_view windowName);

protected:

	void draw() override;

private:

	TextEditor m_textEditor;

};
