#include "MRIFileMenuEditor.h"

void MRI::Editor::FileMenuEditor::Init()
{
	if (!m_saveSuccessPopup) 
	{
		m_saveSuccessPopup = std::make_unique<MRI::Editor::EditorPopup>();
	}

	m_saveSuccessPopup->Init        ();
	m_saveSuccessPopup->SetPopupText(MRI::EditorCommonConstant::k_saveSuccessPopupText.data());
}
void MRI::Editor::FileMenuEditor::DrawEditor()
{
	DrawMenuFile     ();
	UpdateShortCutKey();
	DrawPopUp        ();
}

void MRI::Editor::FileMenuEditor::DrawMenuFile()
{
	if (!m_saveSuccessPopup) { return; }
	
	if (ImGui::BeginMenu("File"))
	{
		// "PopupText"がから文字列じゃなければ実行
		if (ImGui::MenuItem("Save" , "Ctrl+S")		 && 
			!m_saveSuccessPopup->GetPopupText().empty())
		{
			// すべてのファイル情報をセーブ
			MRI::EditorSceneSaveUtility::Save();
			m_saveSuccessPopup->OpenPopup    ();
		}

		ImGui::EndMenu();
	}
}

void MRI::Editor::FileMenuEditor::UpdateShortCutKey()
{
	if (!m_saveSuccessPopup) { return; }

	if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && 
		ImGui::IsKeyPressed(ImGuiKey_S))
	{
		// すべてのファイル情報をセーブ
		MRI::EditorSceneSaveUtility::Save();
		m_saveSuccessPopup->OpenPopup    ();
	}
}

void MRI::Editor::FileMenuEditor::DrawPopUp()
{
	if (!m_saveSuccessPopup) { return; }
	m_saveSuccessPopup->DrawPopup();
}