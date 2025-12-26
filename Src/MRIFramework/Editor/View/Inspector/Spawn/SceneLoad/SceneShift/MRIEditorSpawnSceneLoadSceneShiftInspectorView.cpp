#include "MRIEditorSpawnSceneLoadSceneShiftInspectorView.h"

void MRI::Editor::EditorSpawnSceneShiftInspectorView::DrawEditor()
{
	if (!ImGui::Begin("SpawnSceneShiftInspector"))
	{
		ImGui::End();
		return;
	}

	DrawAddSceneShiftSelector();
	DrawAddSceneShiftButton  ();
	DrawSceneShiftMap        ();

	ImGui::End();
}

void MRI::Editor::EditorSpawnSceneShiftInspectorView::DrawAddSceneShiftSelector()
{
	auto l_editorView = MRI::Editor::EditorManager::GetInstance().FetchEditorViewCache<MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView>().lock();
	if (!l_editorView) { return; }

	ImGui::SetNextItemWidth(MRI::EditorCommonConstant::k_comboWidth);
	ImGui::BeginGroup      ();

	const auto& l_typeInfoRegistry = MRI::TypeInfoRegistry::GetInstance();

	std::string l_tagName = MRI::CommonConstant::k_stringUnknown.data();
	if (auto l_typeInfo = l_typeInfoRegistry.FetchTypeInfoByID(m_addSceneShiftTag))
	{
		l_tagName = l_typeInfo->k_name.data();
	}

	// もしコンボリストが開かれていなければ"return"
	if (!ImGui::BeginCombo("SceneShiftTagSelector" , l_tagName.c_str()))
	{
		ImGui::EndGroup();
		return;
	}

	// ラジオボタンセレクターで選択されたシーン遷移タグを"m_addSceneShiftTag"に格納
	for (const auto& [l_key , l_value] : l_editorView->GetAllSceneShiftMap())
	{
		bool l_isSelected = m_addSceneShiftTag == l_key;

		auto l_typeInfo = l_typeInfoRegistry.FetchTypeInfoByID(l_key);
		if (!l_typeInfo) 
		{
			ImGui::PopID();
			continue;
		}

		ImGui::PushID(&l_key);

		if (ImGui::RadioButton(l_typeInfo->k_name.data() , l_isSelected))
		{
			ImGui::PopID();
			continue;
		}

		// 選択された項目にカーソルを当てる
		if (l_isSelected)
		{
			ImGui::SetItemDefaultFocus();
		}

		m_addSceneShiftTag = l_key;

		ImGui::PopID();
	}

	ImGui::EndCombo();
	ImGui::EndGroup();
}
void MRI::Editor::EditorSpawnSceneShiftInspectorView::DrawAddSceneShiftButton()
{
	auto l_editorView = MRI::Editor::EditorManager::GetInstance().FetchEditorViewCache<MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView>().lock();
	if (!l_editorView) { return; }

	// ボタンが押されたらシーン遷移タグとその際に読み込むファイルパスをシーンのマップに追加
	if (!MRI::EditorUtility::SameLineButton("Add SceneShiftTag")) { return; }

	// シーン遷移のファイルパスを取得
	const std::string l_filePath = l_editorView->FetchSceneShiftFilePath(m_addSceneShiftTag);

	MRI::SceneManager::GetInstance().RegisterSceneShiftMap(m_addSceneShiftTag , l_filePath);
}
void MRI::Editor::EditorSpawnSceneShiftInspectorView::DrawSceneShiftMap() const
{
	const auto& l_typeInfoRegistry = MRI::TypeInfoRegistry::GetInstance					  ();
	auto&		l_sceneShiftMap    = MRI::SceneManager::GetInstance().GetWorkSceneShiftMap();
	auto        l_itr              = l_sceneShiftMap.begin								  ();

	// 削除ボタン、タグ名とファイルパスを描画
	while (l_itr != l_sceneShiftMap.end())
	{
		auto l_typeInfo = l_typeInfoRegistry.FetchTypeInfoByID(l_itr->first);
		if (!l_typeInfo)
		{
			l_itr++;
			continue;
		}

		ImGui::PushID(std::to_address(l_itr));

		if (const std::string l_tagName = l_typeInfo->k_name.data();
			ImGui::TreeNode(l_tagName.c_str()))
		{
			// デリートボタンが押されたら削除
			if (MRI::EditorUtility::SameLineSmallButton("Delete"))
			{
				l_itr = l_sceneShiftMap.erase(l_itr);
				ImGui::PopID				 ();
				ImGui::TreePop				 ();
				continue;
			}

			ImGui::Text   ("FilePath : %s" , l_itr->second.c_str());
			ImGui::TreePop();
		}

		l_itr++;
		ImGui::PopID();
	}
}