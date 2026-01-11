#include "MRIPrefabControllerConverter.h"

void MRI::Converter::PrefabControllerConverter::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null())								  { return; }
	if (!MRI::JsonUtility::IsArray(a_json , "PrefabMap")) { return; }

	auto l_prefabControllerCache = m_prefabControllerCache.lock();
	if (!l_prefabControllerCache) { return; }

	// プレハブの名前と読み込むプレハブのファイルパスを格納
	for (const auto& l_json : a_json["PrefabMap"])
	{
		if (l_json.is_null()) { continue; }

		auto l_prefab = std::make_shared<MRI::Resource::Prefab>();

		const std::string l_prefabName = l_json.value("PrefabName" , MRI::CommonConstant::k_stringUnknown.data());
		
		// プレハブ本体をデシリアライズしてマップに格納
		l_prefab->Deserialize             (l_json);
		l_prefabControllerCache->AddPrefab(l_prefabName , l_prefab);
	}
}
nlohmann::json MRI::Converter::PrefabControllerConverter::Serialize()
{
	auto l_prefabControllerCache = m_prefabControllerCache.lock();
	if (!l_prefabControllerCache) 
	{
		return nlohmann::json();
	}

	auto l_rootJson  = nlohmann::json       ();
	auto l_jsonArray = nlohmann::json::array();

	// そのシーンで使うプレハブの名前とファイルパスを保存
	for (const auto& [l_key , l_value] : l_prefabControllerCache->GetPrefabMap())
	{
		if (!l_value) { continue; }

		auto l_json = nlohmann::json();
		l_json["PrefabName"] = l_key;
		
		// プレハブ本体をシリアライズして"json::array"に格納
		MRI::JsonUtility::UpdateJson(l_json , l_value->Serialize());
		l_jsonArray.emplace_back    (l_json);
	}

	l_rootJson["PrefabMap"] = l_jsonArray;

	return l_rootJson;
}