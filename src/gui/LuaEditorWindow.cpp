#include <gui/LuaEditorWindow.hpp>
#include <app/ApplicationState.hpp>


LuaEditorWindow::LuaEditorWindow(ZigZag::LuaBehaviour& luaBehaviour, ApplicationState* appState) :
	Window(luaBehaviour.getName()),
	m_appState(appState),
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
	ImGui::PushFont(m_appState->fontLibrary.getCodeFont());
	m_textEditor.Render("text", ImGui::GetContentRegionAvail(), true);
	ImGui::PopFont();
}
