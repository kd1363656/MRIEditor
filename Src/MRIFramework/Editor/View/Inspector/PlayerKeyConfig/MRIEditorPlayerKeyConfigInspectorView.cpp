#include "MRIEditorPlayerKeyConfigInspectorView.h"

void MRI::Editor::EditorPlayerKeyConfigInspectorView::DrawEditor()
{
	if (!ImGui::Begin("PlayerKeyConfigInspector"))
	{
		ImGui::End();
		return;
	}

	DrawAddKeyConfigInfo ();
	DrawKeyConfigMap     ();

	ImGui::End();
}

void MRI::Editor::EditorPlayerKeyConfigInspectorView::DrawAddKeyConfigInfo()
{
	auto l_playerKeyConfigCache = SceneManager::GetInstance().GetPlayerKeyConfigCache().lock();
	if (!l_playerKeyConfigCache) { return; }

	auto& l_keyConfigMap = l_playerKeyConfigCache->GetKeyConfigMap();

	// 入力反応タグを選択
	MRI::EditorUtility::TagRadioButtonSelector<MRI::Tag::InputEventTagBase>("InputEventTagSelector" , m_addInputEventTag , MRI::EditorCommonConstant::k_comboWidth);
	DrawAddVKCodeSelector												   ();

	// ボタンを押していないか格納不可能な値を持っていたら"return"
	if (!MRI::EditorUtility::SameLineButton("Add KeyConfigMap"))        { return; }
	if (m_addInputEventTag == MRI::CommonConstant::k_forbiddenStaticID) { return; }
	if (m_addVKCode        == NULL)									    { return; }

	bool l_isRegistered = false;

	// 同じキーを保存しないようにもし同じキーを保存しようとしたら"return"
	for (const auto& [l_key , l_value] : l_keyConfigMap)
	{
		if (m_addVKCode != l_value) { continue; }
		l_isRegistered = true;
	}

	// 登録されていない仮想キーコードを使っているなら登録
	if (l_isRegistered) { return; }
	l_playerKeyConfigCache->RegisterInputConfig(m_addInputEventTag , m_addVKCode);
}
void MRI::Editor::EditorPlayerKeyConfigInspectorView::DrawAddVKCodeSelector()
{
	ImGui::SetNextItemWidth(MRI::EditorCommonConstant::k_comboWidth);
	ImGui::BeginGroup      ();

	std::string l_nowSelecteKeyCodeName = MRI::VKCodeUtility::GetKeyName(m_addVKCode);
	if (l_nowSelecteKeyCodeName.empty())
	{
		l_nowSelecteKeyCodeName = MRI::CommonConstant::k_stringUnknown.data();
	}

	// もしコンボリストが開かれていなければ早期リターン
	if (!ImGui::BeginCombo("KeyCodeSelector" , l_nowSelecteKeyCodeName.c_str()))
	{
		ImGui::EndGroup();
		return;
	}

	// 仮想キーコードを選択
	for (int l_i = 0; l_i < MRI::CommonConstant::k_vkCodeMaxNum; l_i++)
	{
		// もしから文字列が返ってきたら"continue"
		std::string l_keyCodeName = MRI::VKCodeUtility::GetKeyName(l_i);
		if (l_keyCodeName.empty())
		{
			continue;
		}

		ImGui::PushID(l_i);

		bool l_isSelected = m_addVKCode == l_i;

		if (!ImGui::RadioButton(l_keyCodeName.c_str() , l_isSelected))
		{
			ImGui::PopID();
			continue;
		}

		// 選択された項目にカーソルを当てる
		if (l_isSelected)
		{
			ImGui::SetItemDefaultFocus();
		}

		// 選択された仮想キーコードを格納
		m_addVKCode = l_i;

		ImGui::PopID();
	}

	ImGui::EndCombo();
	ImGui::EndGroup();
}
void MRI::Editor::EditorPlayerKeyConfigInspectorView::DrawKeyConfigMap() const
{
	auto l_playerKeyConfigCache = SceneManager::GetInstance().GetPlayerKeyConfigCache().lock();
	if (!l_playerKeyConfigCache) { return; }

	auto& l_keyConfigMap = l_playerKeyConfigCache->GetWorkKeyConfigMap();
	
	auto l_itr = l_keyConfigMap.begin();
	while (l_itr != l_keyConfigMap.end())
	{
		// "ID"から現在の入力タグの型情報を取得
		auto l_typeInfo = MRI::TypeInfoRegistry::GetInstance().FetchTypeInfoByID(l_itr->first);
		if (!l_typeInfo) 
		{
			l_itr++;
			continue;
		}

		ImGui::PushID(std::to_address(l_itr));

		// タグの名前と仮想キーコードの名前、削除ボタンを描画
		if (ImGui::TreeNode(l_typeInfo->k_name.data()))
		{
			if (MRI::EditorUtility::SameLineSmallButton("Delete"))
			{
				l_itr = l_keyConfigMap.erase(l_itr);
				ImGui::PopID  ();
				ImGui::TreePop();
				continue;
			}

			const std::string l_vkKeyName = MRI::VKCodeUtility::GetKeyName(l_itr->second);
			ImGui::Text													  (l_vkKeyName.c_str());
			
			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}