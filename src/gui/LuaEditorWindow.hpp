#pragma once

#include <ZigZag/LuaBehaviour.hpp>

#include <TextEditor.h>

#include "gui/Window.hpp"


class LuaEditorWindow : public Window
{
public:

	LuaEditorWindow(ZigZag::LuaBehaviour& luaBehaviour);

protected:

	void draw() override;

private:

	TextEditor m_textEditor;
	ZigZag::LuaBehaviour* m_luaBehaviour;

};
