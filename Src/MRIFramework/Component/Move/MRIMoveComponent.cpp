#include "MRIMoveComponent.h"

const MRI::TypeInfo& MRI::Component::MoveComponent::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::MoveComponent>();
}

void MRI::Component::MoveComponent::PostLoadInit()
{
	auto l_ownerCache = MRI::Component::ComponentBase::GetWorkOwnerCache().lock();
	if (!l_ownerCache)		  { return; }
	if (!m_moveComponentMode) { return; }

	m_moveComponentMode->PostLoadInit(l_ownerCache);
}

void MRI::Component::MoveComponent::PreUpdate()
{
	if (!m_moveComponentMode) { return; }
	m_moveComponentMode->PreUpdate();
}
void MRI::Component::MoveComponent::Update()
{
	if (!m_moveComponentMode) { return; }
	m_moveComponentMode->Update();
}

void MRI::Component::MoveComponent::EditPrefabInspector()
{
	MRI::EditorUtility::FactoryRadioButtonSelector<MRI::UniqueFactory::MoveComponentMode>("MoveComponentModeSelector" , m_moveComponentMode);

	if (!m_moveComponentMode) { return; }
	m_moveComponentMode->EditPrefabInspector();
}

void MRI::Component::MoveComponent::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	const auto& l_factory = MRI::UniqueFactory::MoveComponentMode::GetInstance();

	const std::string& l_moveComponentModeName = a_json.value    ("MoveComponentModeName" , std::string());
	m_moveComponentMode						   = l_factory.Create(l_moveComponentModeName);

	if (m_moveComponentMode)
	{
		m_moveComponentMode->Init             ();
		m_moveComponentMode->DeserializePrefab(a_json);
	}
}

nlohmann::json MRI::Component::MoveComponent::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	if (m_moveComponentMode)
	{
		l_rootJson["MoveComponentModeName"] = m_moveComponentMode->GetTypeInfo().k_name.data();
		MRI::JsonUtility::UpdateJson									      (l_rootJson , m_moveComponentMode->SerializePrefab());
	}

	return l_rootJson;
}