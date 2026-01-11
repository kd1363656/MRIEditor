#include "MRISceneManagerConverter.h"

void MRI::Converter::SceneManagerConverter::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null())									  { return; }
	if (!MRI::JsonUtility::IsArray(a_json , "SceneShiftMap")) { return; }
	
	auto& l_sceneManager = MRI::SceneManager::GetInstance();

	// シーン遷移する際に反応するオブザーバーのタグとそのファイルパスを格納
	for (const auto& l_json : a_json["SceneShiftMap"])
	{
		if (l_json.is_null()) { continue; }

		const std::uint32_t l_tag      = MRI::JsonUtility::DeserializeTag(l_json);
		const std::string   l_filePath = l_json.value				     ("SceneShiftFilePath" , std::string());

		l_sceneManager.RegisterSceneShiftMap(l_tag , l_filePath);
	}
}
nlohmann::json MRI::Converter::SceneManagerConverter::Serialize()
{
	auto l_rootJson  = nlohmann::json		();
	auto l_jsonArray = nlohmann::json::array();

	const auto& l_sceneManager = MRI::SceneManager::GetInstance();

	// シーン遷移する際に反応するオブザーバーのタグとそのファイルパスを保存
	for (const auto& [l_key , l_value] : l_sceneManager.GetSceneShiftMap())
	{
		auto l_json = nlohmann::json();

		MRI::JsonUtility::UpdateJson(l_json , MRI::JsonUtility::SerializeTag(l_key));
		l_json["SceneShiftFilePath"] = l_value;

		l_jsonArray.emplace_back(l_json);
	}

	l_rootJson["SceneShiftMap"] = l_jsonArray;

	return l_rootJson;
}