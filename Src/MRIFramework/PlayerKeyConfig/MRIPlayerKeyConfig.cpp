#include "MRIPlayerKeyConfig.h"

void MRI::PlayerKeyConfig::Init()
{
	m_keyConfigMap.clear();
}

void MRI::PlayerKeyConfig::LoadFile()
{
	auto l_rootJson = MRI::FileIOUtility::LoadJsonFile(k_fileIOPath);
	
	// 読み取ったファイルが空なら"return"
	if (l_rootJson.is_null()) { return; }

	// リストに読み取り結果を反映
	Deserialize(l_rootJson);
}
void MRI::PlayerKeyConfig::SaveFile() const
{
	auto l_rootJson = nlohmann::json();

	if (auto l_json = Serialize();
		!l_json.is_null())
	{
		MRI::JsonUtility::UpdateJson(l_rootJson , l_json);
	}

	MRI::FileIOUtility::SaveJsonFile(l_rootJson , k_fileIOPath);
}

void MRI::PlayerKeyConfig::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null())									   { return; }
	if (!MRI::JsonUtility::IsArray(a_json , "InputConfigMap")) { return; }

	// キーコンフィグをロード
	for (const auto& l_json : a_json["InputConfigMap"])
	{
		const std::uint32_t l_tag    = MRI::JsonUtility::DeserializeTag(l_json);
		const int           l_vkCode = l_json.value					   ("VKCode" , NULL);

		RegisterInputConfig(l_tag , l_vkCode);
	}
}
nlohmann::json MRI::PlayerKeyConfig::Serialize() const
{
	auto l_rootJson  = nlohmann::json       ();
	auto l_jsonArray = nlohmann::json::array();

	// キーコンフィグを保存
	for (const auto& [l_key , l_value] : m_keyConfigMap)
	{
		auto l_json = nlohmann::json();

		MRI::JsonUtility::UpdateJson(l_json , MRI::JsonUtility::SerializeTag(l_key));
		l_json["VKCode"] = l_value;

		l_jsonArray.emplace_back(l_json);
	}

	l_rootJson["InputConfigMap"] = l_jsonArray;

	return l_rootJson;
}

void MRI::PlayerKeyConfig::RegisterInputConfig(const std::uint32_t a_tag , const int a_vkCode)
{
	if (a_tag == MRI::CommonConstant::k_invalidStaticID) { return; }
	m_keyConfigMap.try_emplace(a_tag , a_vkCode);
}