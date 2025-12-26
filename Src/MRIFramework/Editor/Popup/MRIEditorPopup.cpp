#include "MRIEditorPopup.h"

void MRI::Editor::EditorPopup::Init()
{
	m_popupText = MRI::CommonConstant::k_stringEmpty.data();
	m_isAppear  = false;
}

void MRI::Editor::EditorPopup::OpenPopup()
{
	// ポップアップのテキストがなければ"return"
	if (m_popupText.empty()) { return; }
	m_isAppear = true;
}
void MRI::Editor::EditorPopup::DrawPopup()
{
	// 文字列が空なら描画しない
	if (m_popupText.empty()) { return; }

	// もし"OpenPopup"が呼び出されていたら実行
	if (m_isAppear)
	{
		ImGui::OpenPopup(m_popupText.c_str());
		m_isAppear = false;
	}

	// セーブ完了モーダルポップアップの描画
	if (ImGui::BeginPopupModal(m_popupText.c_str() , nullptr , ImGuiWindowFlags_AlwaysAutoResize))
	{
		// 初期フォーカスを"OK"ボタンに
		ImGui::SetItemDefaultFocus();

		// "OK"ボタンを描画し、押されたらポップアップを閉じる
		if (ImGui::Button("============= \"OK\" =============") || 
			ImGui::IsKeyPressed(ImGuiKey_Enter))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}