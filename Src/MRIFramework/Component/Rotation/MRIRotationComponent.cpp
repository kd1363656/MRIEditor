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
	auto l_ownerCache = MRI::Component::ComponentBase::GetWorkOwnerCache().lock();
	if (!l_ownerCache) { return; }

	// 回転のモード切替を行う
	if (const bool l_isCreate = MRI::EditorUtility::FactoryRadioButtonSelector<MRI::SharedFactory::RotationComponentMode>("RotationComponentMode", m_rotationComponentMode);
		l_isCreate && m_rotationComponentMode)
	{
		// インスタンスが生成されたら初期化
		m_rotationComponentMode->Init        ();
		m_rotationComponentMode->PostLoadInit(l_ownerCache);
	}

	if (!m_rotationComponentMode) { return; }
	m_rotationComponentMode->EditPrefabInspector();
}

void MRI::Component::RotationComponent::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::JsonUtility::DeserializeInstancePrefab<MRI::SharedFactory::RotationComponentMode>(a_json , "RotationComponentModeName" , m_rotationComponentMode);
}

nlohmann::json MRI::Component::RotationComponent::SerializePrefab()
{
	if (!m_rotationComponentMode)
	{
		return nlohmann::json();
	}

	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::JsonUtility::SerializeInstancePrefab("RotationComponentModeName" , m_rotationComponentMode));

	return l_rootJson;
}