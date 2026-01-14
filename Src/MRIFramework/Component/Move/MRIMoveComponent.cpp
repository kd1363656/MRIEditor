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
	MRI::EditorUtility::FactoryRadioButtonSelector<MRI::SharedFactory::MoveComponentMode>("MoveComponentModeSelector" , m_moveComponentMode);

	if (!m_moveComponentMode) { return; }
	m_moveComponentMode->EditPrefabInspector();
}

void MRI::Component::MoveComponent::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::JsonUtility::DeserializeInstancePrefab<MRI::SharedFactory::MoveComponentMode>(a_json , "MoveComponentModeName" , m_moveComponentMode);
}

nlohmann::json MRI::Component::MoveComponent::SerializePrefab()
{
	if(!m_moveComponentMode)
	{
		return nlohmann::json();
	}

	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::JsonUtility::SerializeInstancePrefab("MoveComponentModeName" , m_moveComponentMode));
	
	return l_rootJson;
}