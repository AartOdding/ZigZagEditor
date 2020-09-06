#pragma once

#include <ZigZag/LuaBehaviour.hpp>

#include <TextEditor.h>

#include "gui/Window.hpp"


struct ApplicationState;


class LuaEditorWindow : public Window
{
public:

	LuaEditorWindow(ZigZag::LuaBehaviour& luaBehaviour, ApplicationState* appState);

protected:

	void draw() override;

private:

	ApplicationState* m_appState;

	TextEditor m_textEditor;
	ZigZag::LuaBehaviour* m_luaBehaviour;

};
