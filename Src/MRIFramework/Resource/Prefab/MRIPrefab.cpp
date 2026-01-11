#include "MRIPrefab.h"

void MRI::Resource::Prefab::LoadPrefab()
{
	if (m_filePath.empty()) { return; }

	m_json = MRI::FileIOUtility::LoadJsonFile(m_filePath);
}
void MRI::Resource::Prefab::SavePrefab() const
{
	if (m_filePath.empty()) { return; }

	auto l_gameObjectCache = m_gameObjectCache.lock();
	if (!l_gameObjectCache) { return; }

	// キャッシュしているゲームオブジェクトからプレハブを取得
	auto l_json = l_gameObjectCache->SerializePrefab();
	if (l_json.is_null()) { return; }

	MRI::FileIOUtility::SaveJsonFile(l_json , m_filePath);
}

void MRI::Resource::Prefab::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }
	
	m_filePath = a_json.value("FilePath" , std::string());
}

nlohmann::json MRI::Resource::Prefab::Serialize() const
{
	auto l_rootJson = nlohmann::json();

	// キャッシュしているゲームオブジェクトからシリアライズ情報を取得
	l_rootJson["FilePath"] = m_filePath;

	return l_rootJson;
}