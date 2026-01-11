#include "MRISceneConverter.h"

void MRI::Converter::SceneConverter::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null())									   { return; }
	if (!MRI::JsonUtility::IsArray(a_json , "GameObjectList")) { return; }

	// スポーン情報を読み込む
	for (const auto& l_json : a_json["GameObjectList"])
	{
		if (l_json.is_null()) { continue; }
		DeserializeGameObject(l_json);
	}

	// "Prefab"変更対象ゲームオブジェクトを決める(各ゲームオブジェクトにつき一つ)
	DecidePrefabGameObject();
}
nlohmann::json MRI::Converter::SceneConverter::Serialize()
{
	auto l_sceneCache = m_sceneCache.lock();
	if (!l_sceneCache) 
	{
		return nlohmann::json();
	}

	auto l_rootJson  = nlohmann::json       ();
	auto l_jsonArray = nlohmann::json::array();

	// スポーン情報のみ保存
	for (const auto& l_gameObject : l_sceneCache->GetGameObjectList())
	{
		// ヌルポインタかそれとも親を思っているならデシリアライズしない
		// 親がいる場合親が全ての子をデシリアライズするからである
		if (!l_gameObject || l_gameObject->HasParent()) { continue; }
		
		// ゲームオブジェクトからシリアライズした"json"ファイルが"null"なら"continue"
		const auto& l_json = l_gameObject->SerializeSpawn();
		if (l_json.is_null()) { continue; }

		l_jsonArray.emplace_back(l_json);
	}

	l_rootJson["GameObjectList"] = l_jsonArray;

	return l_rootJson;
}

void MRI::Converter::SceneConverter::DeserializeGameObject(const nlohmann::json& a_json) const
{
	if (a_json.is_null()) { return; }

	auto l_sceneCache = m_sceneCache.lock();
	if (!l_sceneCache) { return; }

	// ゲームオブジェクトを作成し読み取った"json"でデシリアライズ
	auto l_gameObject = MRI::GameObjectUtility::CreateGameObject();
	l_gameObject->Init											();
	l_gameObject->Deserialize									(a_json);

	// シーンインゲームオブジェクトを追加
	l_sceneCache->AddGameObject(l_gameObject);
}
void MRI::Converter::SceneConverter::DecidePrefabGameObject() const
{
	auto l_sceneCache            = m_sceneCache.lock		   ();
	auto l_prefabControllerCache = m_prefabControllerCache.lock();

	if (!l_sceneCache || !l_prefabControllerCache) { return; }

	// まだプレハブに格納されていない各ゲームオブジェクトを各種類一体だけ格納
	// ルートゲームオブジェクトのキャッシュのみ取得
	for (const auto& l_gameObject : l_sceneCache->GetGameObjectList())
	{
		// ヌルポインタ化子ゲームオブジェクトなら処理を飛ばす
		if (!l_gameObject)             { continue; }
		if (l_gameObject->HasParent()) { continue; }

		// プレハブ名からプレハブを取得
		auto l_prefab = l_prefabControllerCache->FetchPrefabCache(l_gameObject->GetPrefabName()).lock();
		if (!l_prefab) { continue; }

		if (!l_prefab->GetGameObjectCache().expired()) { continue; }

		l_prefab->SetGameObjectCache(l_gameObject);
	}
}