#include <gui/LuaEditorWindow.hpp>


LuaEditorWindow::LuaEditorWindow(std::string_view windowName)
	: Window(windowName)
{
	m_textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
	m_textEditor.SetText("");
}


void LuaEditorWindow::draw()
{
	m_textEditor.Render("text", ImGui::GetContentRegionAvail(), true);
}
