#include "MRIRotationComponent.h"

const MRI::TypeInfo& MRI::Component::RotationComponent::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::RotationComponent>();
}

void MRI::Component::RotationComponent::PostLoadInit()
{
	auto l_ownerCache = MRI::Component::ComponentBase::GetWorkOwnerCache().lock();

	if (!l_ownerCache)            { return; }
	if (!m_rotationComponentMode) { return; }

	m_rotationComponentMode->PostLoadInit(l_ownerCache);
}

void MRI::Component::RotationComponent::PreUpdate()
{
	if (!m_rotationComponentMode) { return; }
	m_rotationComponentMode->PreUpdate();
}
void MRI::Component::RotationComponent::Update()
{
	if (!m_rotationComponentMode) { return; }
	m_rotationComponentMode->Update();
}

void MRI::Component::RotationComponent::EditPrefabInspector()
{
	// 回転のモード切替を行う
	MRI::EditorUtility::FactoryRadioButtonSelector<MRI::UniqueFactory::RotationComponentMode>("RotationComponentMode" , m_rotationComponentMode);

	if (!m_rotationComponentMode) { return; }
	m_rotationComponentMode->EditPrefabInspector();
}

void MRI::Component::RotationComponent::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	const auto& l_factory = MRI::UniqueFactory::RotationComponentMode::GetInstance();

	const std::string& l_rotationComponentModeName = a_json.value("RotationComponentModeName" , std::string());
	m_rotationComponentMode						   = l_factory.Create(l_rotationComponentModeName);

	if (m_rotationComponentMode)
	{
		m_rotationComponentMode->DeserializePrefab(a_json);
	}
}

nlohmann::json MRI::Component::RotationComponent::SerializePrefab()
{
	if (!m_rotationComponentMode)
	{
		return nlohmann::json();
	}

	auto l_rootJson = nlohmann::json();

	l_rootJson["RotationComponentModeName"] = m_rotationComponentMode->GetTypeInfo().k_name.data();
	MRI::JsonUtility::UpdateJson											      (l_rootJson , m_rotationComponentMode->SerializePrefab());

	return l_rootJson;
}