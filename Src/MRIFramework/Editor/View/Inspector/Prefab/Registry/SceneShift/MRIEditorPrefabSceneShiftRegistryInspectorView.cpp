#include "MRIEditorPrefabSceneShiftRegistryInspectorView.h"

void MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView::DrawEditor()
{
	if (!ImGui::Begin("PrefabSceneShiftRegistryInspector"))
	{
		ImGui::End();
		return;
	}

	ImGui::PushID(GetTypeID());

	DrawAddSceneShiftInfo();
	DrawAllSceneShiftMap ();
	
	ImGui::PopID();
	ImGui::End  ();
}

void MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView::LoadFile()
{
	auto l_rootJson = MRI::FileIOUtility::LoadJsonFile(k_fileIOPath.data());

	if (l_rootJson.is_null())										 { return; }
	if (!MRI::JsonUtility::IsArray(l_rootJson , "AllSceneShiftMap")) { return; }

	// シーン遷移する際に反応するタグとファイルパスを読み込む
	for (const auto& l_json : l_rootJson["AllSceneShiftMap"])
	{
		std::uint32_t l_tag      = MRI::JsonUtility::DeserializeTag(l_json);
		std::string   l_filePath = l_json.value					   ("FilePath" , MRI::CommonConstant::k_stringEmpty.data());

		RegisterSceneShiftMap(l_tag , l_filePath);
	}
}
void MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView::SaveFile()
{
	auto l_rootJson  = nlohmann::json       ();
	auto l_jsonArray = nlohmann::json::array();

	// タグとファイルパスを保存
	for (const auto& [l_key , l_value] : m_allSceneShiftMap)
	{
		auto l_json = nlohmann::json();

		MRI::JsonUtility::UpdateJson(l_json , MRI::JsonUtility::SerializeTag(l_key));
		l_json["FilePath"] = l_value;

		l_jsonArray.emplace_back(l_json);
	}

	l_rootJson["AllSceneShiftMap"] = l_jsonArray;

	if (!l_rootJson.is_null())
	{
		MRI::FileIOUtility::SaveJsonFile(l_rootJson , k_fileIOPath.data());
	}
}

std::string MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView::FetchSceneShiftFilePath(const std::uint32_t a_tag) const
{
	auto l_itr = m_allSceneShiftMap.find(a_tag);
	if (l_itr == m_allSceneShiftMap.end()) 
	{
		return MRI::CommonConstant::k_stringUnknown.data();
	}

	return l_itr->second;
}

void MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView::DrawAddSceneShiftInfo()
{
	// シーン遷移用タグのみのセレクター
	MRI::EditorUtility::TagRadioButtonSelector<MRI::Tag::SceneShiftEventTagBase>("SceneShiftEventRegistrySelector" , m_addSceneShiftTag , MRI::EditorCommonConstant::k_comboWidth);

	// ボタンが押されなければ"return"
	// シーンシフトイベントを登録しそのイベントに該当するシーンの"json"のファイルパスを決める
	if (!MRI::EditorUtility::SameLineButton("Add SceneShiftEventRegistry")) { return; }

	// 既に登録されているなら登録しない
	if (m_allSceneShiftMap.contains(m_addSceneShiftTag))
	{
		const auto& l_typeInfoRegistry = MRI::TypeInfoRegistry::GetInstance  ();
		const auto  l_typeInfo         = l_typeInfoRegistry.FetchTypeInfoByID(m_addSceneShiftTag);

		if (!l_typeInfo) { return; }

		const std::string l_typeInfoName = l_typeInfo->k_name.data();

		MRI_ADD_LOG("[%s]は既に登録されています。" , l_typeInfoName.c_str());
		return;
	}

	// フィルターとファイルパスを保存するローカル変数を作成
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

	// マップに格納
	RegisterSceneShiftMap(m_addSceneShiftTag , l_filePath);
	// 今マップに格納したということはシーンで使う予定のマップである可能性が高いのでシーンで反応するイベントとして
	// シーンのマップに格納
	MRI::SceneManager::GetInstance().RegisterSceneShiftMap(m_addSceneShiftTag , l_filePath);
}
void MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView::DrawAllSceneShiftMap()
{
	const auto& l_typeInfoRegistry = MRI::TypeInfoRegistry::GetInstance();
	auto		l_itr			   = m_allSceneShiftMap.begin		   ();

	// 削除ボタン、タグ名とファイルパスを描画
	while (l_itr != m_allSceneShiftMap.end())
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
				l_itr = m_allSceneShiftMap.erase(l_itr);
				ImGui::PopID  ();
				ImGui::TreePop();
				continue;
			}

			ImGui::Text   ("FilePath : %s" , l_itr->second.c_str());
			ImGui::TreePop();
		}
		
		l_itr++;
		ImGui::PopID();
	}
}

void MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView::RegisterSceneShiftMap(const uint32_t a_tag, const std::string& a_filePath)
{
	m_allSceneShiftMap.try_emplace(a_tag , a_filePath);
}