#include "MRIPrefabController.h"

void MRI::Resource::PrefabController::Init()
{
	if (!m_prefabControllerConverter)
	{
		m_prefabControllerConverter = std::make_unique<MRI::Converter::PrefabControllerConverter>();
	}

	m_prefabControllerConverter->SetPrefabControllerCache(weak_from_this());

	if (!m_prefabControllerFileIO)
	{
		m_prefabControllerFileIO = std::make_unique<MRI::FileIO::PrefabControllerFileIO>();
	}

	m_prefabControllerFileIO->SetPrefabControllerCache(weak_from_this());
}

// ファイルパスからプレハブの保存、読み込みを行う
void MRI::Resource::PrefabController::LoadFile()
{
	if (!m_prefabControllerFileIO) { return; }
	m_prefabControllerFileIO->LoadFile();
}
void MRI::Resource::PrefabController::SaveFile()
{
	if (!m_prefabControllerFileIO) { return; }
	m_prefabControllerFileIO->SaveFile();
}

void MRI::Resource::PrefabController::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null() || !m_prefabControllerConverter) { return; }
	
	// プレハブの名前とプレハブのファイルパスの読み込みを行う
	m_prefabControllerConverter->Deserialize(a_json);
}

nlohmann::json MRI::Resource::PrefabController::Serialize() const
{
	auto l_rootJson = nlohmann::json();

	if (!m_prefabControllerConverter) 
	{
		return nlohmann::json();
	}

	MRI::JsonUtility::UpdateJson(l_rootJson , m_prefabControllerConverter->Serialize());

	return l_rootJson;
}

void MRI::Resource::PrefabController::RemovePrefab(const std::string& a_prefabKey)
{
	auto l_itr = m_prefabMap.find(a_prefabKey);
	if (l_itr == m_prefabMap.end()) { return; }

	if (auto l_prefab = l_itr->second;
		!l_prefab)
	{
		return;
	}

	m_prefabMap.erase(l_itr);
}

std::weak_ptr<MRI::Resource::Prefab> MRI::Resource::PrefabController::FetchPrefabCache(const std::string_view& a_prefabName)
{
	auto l_itr = m_prefabMap.find(a_prefabName);
	if (l_itr == m_prefabMap.end())
	{
		return std::weak_ptr<MRI::Resource::Prefab>();
	}

	// ヌルチェック
	auto l_prefab = l_itr->second;
	if (!l_prefab)
	{
		return std::weak_ptr<MRI::Resource::Prefab>();
	}

	return l_prefab;
}