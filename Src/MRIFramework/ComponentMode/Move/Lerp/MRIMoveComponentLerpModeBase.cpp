#include "MRIMoveComponentLerpModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::MoveComponentLerpModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentLerpModeBase>();
}

void MRI::ComponentMode::MoveComponentLerpModeBase::EditPrefabInspector()
{
	MRI::ComponentMode::MoveComponentModeBase::EditPrefabInspector();

	if (MRI::EditorUtility::FactoryRadioButtonSelector<MRI::SharedFactory::InterpolatorModifier>("InterpolatorModifierSelector" , m_interpolatorModifier) && m_interpolatorModifier)
	{
		m_interpolatorModifier->Init();
	}

	if (!m_interpolatorModifier) { return; }
	m_interpolatorModifier->EditPrefabInspector();
}

void MRI::ComponentMode::MoveComponentLerpModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::ComponentMode::MoveComponentModeBase::DeserializePrefab(a_json);

	MRI::JsonUtility::DeserializeInstancePrefab<MRI::SharedFactory::InterpolatorModifier>(a_json , "InterpolatorModifierName" , m_interpolatorModifier);
}

nlohmann::json MRI::ComponentMode::MoveComponentLerpModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::ComponentMode::MoveComponentModeBase::SerializePrefab());

	if (!m_interpolatorModifier) 
	{
		return nlohmann::json();
	}

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::JsonUtility::SerializeInstancePrefab("InterpolatorModifierName" , m_interpolatorModifier));

	return l_rootJson;
}