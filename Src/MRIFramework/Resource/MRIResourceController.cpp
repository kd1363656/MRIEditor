#include "MRIResourceController.h"

void MRI::Resource::ResourceController::Init() 
{
	if (!m_prefabController)
	{
		m_prefabController = std::make_shared<MRI::Resource::PrefabController>();
	}

	m_prefabController->Init();
}

void MRI::Resource::ResourceController::LoadFile()
{
	if (!m_prefabController) { return; }
	m_prefabController->LoadFile();
}
void MRI::Resource::ResourceController::SaveFile()
{
	if (!m_prefabController) { return; }
	m_prefabController->SaveFile();
}

void MRI::Resource::ResourceController::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null() || !m_prefabController) { return; }
	
	m_prefabController->Deserialize(a_json);
}
nlohmann::json MRI::Resource::ResourceController::Serialize() const
{
	auto l_rootJson = nlohmann::json();

	if (!m_prefabController)
	{
		return nlohmann::json();
	}

	MRI::JsonUtility::UpdateJson(l_rootJson , m_prefabController->Serialize());
	return l_rootJson;
}

void MRI::Resource::ResourceController::ClearAsset() const
{
	KdAssets::Instance().ClearData(true);
}