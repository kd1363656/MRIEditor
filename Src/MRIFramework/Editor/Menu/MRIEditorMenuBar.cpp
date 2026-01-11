#include "MRIEditorMenuBar.h"

void MRI::Editor::EditorMenuBar::Init()
{
	RegisterMenu<MRI::Editor::FileMenuEditor>();

	for (const auto& l_menu : m_editorMenuList)
	{
		if (!l_menu) { continue; }
		l_menu->Init();
	}
}
void MRI::Editor::EditorMenuBar::DrawEditor() const
{
	if (!ImGui::BeginMainMenuBar()) { return; }
	
	// メニューバーの各項目を描画
	for (const auto& l_menu : m_editorMenuList)
	{
		if (!l_menu) { continue; }
		l_menu->DrawEditor();
	}

	ImGui::EndMainMenuBar();
}