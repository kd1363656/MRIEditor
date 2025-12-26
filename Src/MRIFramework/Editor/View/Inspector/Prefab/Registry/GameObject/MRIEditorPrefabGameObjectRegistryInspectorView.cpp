#include "MRIEditorPrefabGameObjectRegistryInspectorView.h"

void MRI::Editor::EditorPrefabGameObjectRegistryInspectorView::Init()
{
	m_allPrefabMap.clear();

	if (!m_saveSuccessPopup)
	{
		m_saveSuccessPopup = std::make_unique<MRI::Editor::EditorPopup>();
	}

	m_saveSuccessPopup->Init		();
	m_saveSuccessPopup->SetPopupText(MRI::EditorCommonConstant::k_saveSuccessPopupText.data());

	m_prefabNameBuffer = MRI::CommonConstant::k_stringUnknown.data();
}
void MRI::Editor::EditorPrefabGameObjectRegistryInspectorView::DrawEditor()
{
	if (!ImGui::Begin("PrefabGameObjectRegistryInspector"))
	{	
		ImGui::End();
		return;
	}

	DrawAddPrefabMapInfo();
	DrawAllPrefabMap    ();
	UpdatePopUp         ();

	ImGui::End();
}

void MRI::Editor::EditorPrefabGameObjectRegistryInspectorView::LoadFile()
{
	auto l_rootJson = MRI::FileIOUtility::LoadJsonFile(k_fileIOPath.data());

	if (l_rootJson.is_null())									 { return; }
	if (!MRI::JsonUtility::IsArray(l_rootJson , "AllPrefabMap")) { return; }

	// 全てのプレハブの名前とロードするファイルのパスを読み込む
	for (const auto& l_json : l_rootJson["AllPrefabMap"])
	{
		if (l_json.is_null()) { continue; }

		const std::string l_prefabName     = l_json.value("PrefabName" , MRI::CommonConstant::k_stringUnknown.data());
		const std::string l_prefabFilePath = l_json.value("PrefabName" , MRI::CommonConstant::k_stringUnknown.data());

		RegisterPrefabMap(l_prefabName , l_prefabFilePath);
	}
}
void MRI::Editor::EditorPrefabGameObjectRegistryInspectorView::SaveFile()
{
	auto l_rootJson  = nlohmann::json       ();
	auto l_jsonArray = nlohmann::json::array();

	// 全てのプレハブの名前とロードするファイルのパスを保存
	for (const auto& [l_key , l_value] : m_allPrefabMap)
	{
		auto l_json = nlohmann::json();

		l_json["PrefabName"]     = l_key;
		l_json["PrefabFilePath"] = l_value;

		l_jsonArray.emplace_back(l_json);
	}

	l_rootJson["AllPrefabMap"] = l_jsonArray;

	if (!l_rootJson.is_null())
	{
		MRI::FileIOUtility::SaveJsonFile(l_rootJson , k_fileIOPath.data());
	}
}

std::string MRI::Editor::EditorPrefabGameObjectRegistryInspectorView::FetchPrefabFilePath(const std::string& a_prefabName)
{
	// プレハブの名前からロードするファイルパスを取得
	auto l_itr = m_allPrefabMap.find(a_prefabName);
	if (l_itr == m_allPrefabMap.end())
	{
		return MRI::CommonConstant::k_stringEmpty.data();
	}

	return l_itr->second;
}

void MRI::Editor::EditorPrefabGameObjectRegistryInspectorView::DrawAddPrefabMapInfo()
{
	// 追加するプレハブの名前を入力
	MRI::EditorUtility::InputTextWithCustomWidth("PrefabName" , m_prefabNameBuffer);

	// ボタンがクリックされるまで実行しない
	if (!MRI::EditorUtility::SameLineButton("Add PrefabMap")) { return; }

	// 既に登録されているなら登録しない
	if (m_allPrefabMap.contains(m_prefabNameBuffer))
	{
		MRI_ADD_LOG("[%s]は既に登録されています。" , m_prefabNameBuffer.c_str());
		return;
	}

	// ファイルパスを保存するためのフィルターとファイルパスのローカル変数
	std::string l_filter   = MRI::CommonConstant::k_jsonFilePathFilter;
	std::string l_filePath = MRI::CommonConstant::k_stringEmpty.data();

	// しっかり保存されていなければ追加しない
	if (!MRI::FileIOUtility::SaveFileDialog(l_filePath       , 
		                                    "ファイルを保存" , 
										    l_filter.c_str() , 
										    "json"))
	{
		return;
	}

	RegisterPrefabMap								(m_prefabNameBuffer , l_filePath);
	MRI::EditorUtility::CreateAndRegisterScenePrefab(m_prefabNameBuffer , l_filePath);
}

void MRI::Editor::EditorPrefabGameObjectRegistryInspectorView::DrawAllPrefabMap()
{
	if (!m_saveSuccessPopup) { return; }

	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) { return; }

	auto l_itr = m_allPrefabMap.begin();
	// 名前とファイルパス、削除ボタンを描画
	while (l_itr != m_allPrefabMap.end())
	{
		ImGui::PushID(std::to_address(l_itr));

		const std::string l_prefabName     = l_itr->first;
		const std::string l_prefabFilePath = l_itr->second;

		if (ImGui::TreeNode(l_prefabName.c_str()))
		{
			// シーンに依存しないプレハブの名前とファイルパスを保存する
			// このレジストリから削除するから削除するすなわち
			// あらゆるシーンで管理しないプレハブなのでシーンで管理しているかつ
			// 同じプレハブ名のプレハブを削除する
			if (MRI::EditorUtility::SameLineSmallButton("Delete"))
			{
				// プレハブをシーンから取り除き、ファイルを削除
				l_prefabControllerCache->RemovePrefab(l_itr->first);
				MRI::FileIOUtility::DeleteFileByPath (l_itr->second);

				// シーン全体のリストから該当する名前のゲームオブジェクトを削除
				MRI::EditorUtility::DeleteGameObjectIfPrefabRemoved(l_itr->first);

				// 最後にマップからも削除をおこない結果を判定するためにセーブ
				l_itr = m_allPrefabMap.erase(l_itr);

				// ファイルのセーブを行う(削除したという設定を判定)
				MRI::EditorSceneSaveUtility::Save();
				m_saveSuccessPopup->OpenPopup    ();

				ImGui::TreePop();
				ImGui::PopID  ();
				continue;
			}

			ImGui::Text   ("PrefabFilePath : %s" , l_prefabFilePath.c_str());
			ImGui::TreePop();
		}

		l_itr++;
		ImGui::PopID();
	}
}

void MRI::Editor::EditorPrefabGameObjectRegistryInspectorView::UpdatePopUp()
{
	if (!m_saveSuccessPopup) { return; }
	m_saveSuccessPopup->DrawPopup();
}

void MRI::Editor::EditorPrefabGameObjectRegistryInspectorView::RegisterPrefabMap(const std::string& a_prefabName , const std::string& a_prefabFilePath)
{
	// プレハブ名とそのプレハブの保存場所を格納
	m_allPrefabMap.try_emplace(a_prefabName , a_prefabFilePath);
}