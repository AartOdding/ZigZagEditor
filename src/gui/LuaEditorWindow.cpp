#include <gui/LuaEditorWindow.hpp>


LuaEditorWindow::LuaEditorWindow(ZigZag::LuaBehaviour& luaBehaviour) : 
	Window(luaBehaviour.getName()),
	m_luaBehaviour(&luaBehaviour)
{
	m_textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
	m_textEditor.SetText(luaBehaviour.getScript());
}


void LuaEditorWindow::draw()
{
	if (ImGui::Button("Save"))
	{
		m_luaBehaviour->loadScript(m_textEditor.GetText());
	}
	m_textEditor.Render("text", ImGui::GetContentRegionAvail(), true);
}
