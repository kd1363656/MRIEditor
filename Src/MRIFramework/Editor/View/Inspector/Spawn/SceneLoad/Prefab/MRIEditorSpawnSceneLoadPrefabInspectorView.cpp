#include "MRIEditorSpawnSceneLoadPrefabInspectorView.h"

void MRI::Editor::EditorSpawnSceneLoadPrefabInspectorView::DrawEditor()
{
	if (!ImGui::Begin("SpawnSceneLoadPrefabInspector"))
	{
		ImGui::End();
		return;
	}

	DrawAddSceneLoadPrefabMapSelector();
	DrawAddSceneLoadPrefabMapButton  ();
	DrawSceneLoadPrefabMap           ();

	ImGui::End();
}

void MRI::Editor::EditorSpawnSceneLoadPrefabInspectorView::DrawAddSceneLoadPrefabMapSelector()
{
	auto l_editorViewCache = MRI::Editor::EditorManager::GetInstance().FetchEditorViewCache<MRI::Editor::EditorPrefabGameObjectRegistryInspectorView>().lock();
	if (!l_editorViewCache) { return; }

	ImGui::SetNextItemWidth(MRI::EditorCommonConstant::k_comboWidth);
	ImGui::BeginGroup      ();

	if (!ImGui::BeginCombo("SceneLoadPrefabSelector" , m_addPrefabName.c_str()))
	{
		ImGui::EndGroup();
		return;
	}

	// 全てのプレハブを管理しているレジストリから取得するプレハブの名前を
	// "m_addPrefabName"に格納
	for (const auto& [l_key , l_value] : l_editorViewCache->GetAllPrefabMap())
	{
		ImGui::PushID(&l_value);

		bool l_isSelected = l_key == m_addPrefabName;

		if (!ImGui::RadioButton(l_key.c_str() , l_isSelected))
		{
			ImGui::PopID();
			continue;
		}

		// 選択された項目にカーソルを当てる
		if (l_isSelected)
		{
			ImGui::SetItemDefaultFocus();
		}

		m_addPrefabName = l_key;

		ImGui::PopID();
	}

	ImGui::EndCombo();
	ImGui::EndGroup();
}
void MRI::Editor::EditorSpawnSceneLoadPrefabInspectorView::DrawAddSceneLoadPrefabMapButton()
{
	auto l_editorViewCache = MRI::Editor::EditorManager::GetInstance().FetchEditorViewCache<MRI::Editor::EditorPrefabGameObjectRegistryInspectorView>().lock();
	if (!l_editorViewCache) { return; }

	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) { return; }

	// ボタンが押されたら該当するプレハブのファイルパスを"Scene"の"PrefabController"のプレハブ読み込みマップに追加
	if (!MRI::EditorUtility::SameLineButton("Add SceneLoadPrefab")) { return; }

	// シーンで呼び出すプレハブが既に登録されていれば実行しない
	if (!l_prefabControllerCache->FetchPrefabCache(m_addPrefabName).expired()) 
	{ 
		MRI_ADD_LOG("[$s]はすでにシーンで読み込むプレハブのマップに登録されています。" , m_addPrefabName.c_str());
		return;
	}

	// ファイルパスが空ならプレハブが登録されていない証拠 
	const std::string l_filePath = l_editorViewCache->FetchPrefabFilePath(m_addPrefabName);
	if (l_filePath.empty()) 
	{
		MRI_ADD_LOG("シーンで読み込むプレハブ[$s]を追加しようとしましたが出来ませんでした。" , l_filePath.c_str());
		return;
	}

	// TODO
	// シーンでロードするプレハブとしてプレハブとゲームオブ絵ジェクトを作成しマップに格納
	MRI::EditorUtility::CreateAndRegisterScenePrefab(m_addPrefabName , l_filePath);
}
void MRI::Editor::EditorSpawnSceneLoadPrefabInspectorView::DrawSceneLoadPrefabMap()
{
	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) { return; }

	auto& l_prefabMap = l_prefabControllerCache->GetWorkPrefabMap();
 
	// マップを走査してシーンで読み込むプレハブの名前とファイルパス、
	// 必要のないものプレハブを消すボタンを描画
	auto l_itr = l_prefabMap.begin();
	while (l_itr != l_prefabMap.end()) 
	{
		auto l_prefab = l_itr->second;
		if (!l_prefab) 
		{
			l_itr++;
			continue;
		}

		ImGui::PushID(l_prefab.get());

		if (const std::string l_prefabName = l_itr->first;
			ImGui::TreeNode(l_prefabName.c_str()))
		{
			if (MRI::EditorUtility::SameLineSmallButton("Delete"))
			{
				ImGui::TreePop           ();
				ImGui::PopID             ();
				l_itr = l_prefabMap.erase(l_itr);

				// プレハブ名に該当するゲームオブジェクトを削除
				MRI::EditorUtility::DeleteGameObjectIfPrefabRemoved(l_prefabName);
				continue;
			}

			ImGui::Text   ("PrefabFilePath : %s" , l_prefab->GetFilePath().data());
			ImGui::TreePop();
		}

		l_itr++;
		ImGui::PopID();
	}
}