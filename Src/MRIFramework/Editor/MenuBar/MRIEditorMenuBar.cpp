#include "MRIEditorMenuBar.h"

void MRI::Editor::EditorMenuBar::Init()
{
	RegisterMenu(std::make_unique<MRI::Editor::FileMenuEditor>());

	for (const auto& l_menu : m_editorMenuList)
	{
		if (!l_menu) { continue; }
		l_menu->Init();
	}
}
void MRI::Editor::EditorMenuBar::DrawEditor() const
{
	if (!ImGui::BeginMainMenuBar()) { return; }
	
	for (const auto& l_menu : m_editorMenuList)
	{
		if (!l_menu) { continue; }
		l_menu->DrawEditor();
	}

	ImGui::EndMainMenuBar();
}

void MRI::Editor::EditorMenuBar::RegisterMenu(std::unique_ptr<MRI::Editor::EditorMenuBase> a_menu)
{
	m_editorMenuList.emplace_back(std::move(a_menu));
}