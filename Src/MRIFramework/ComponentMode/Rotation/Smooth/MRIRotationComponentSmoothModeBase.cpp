#include "MRIRotationComponentSmoothModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentSmoothModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentSmoothModeBase>();
}

void MRI::ComponentMode::RotationComponentSmoothModeBase::EditPrefabInspector()
{
	MRI::ComponentMode::RotationComponentModeBase::EditPrefabInspector();

	if (MRI::EditorUtility::FactoryRadioButtonSelector<MRI::SharedFactory::InterpolatorModifier>("InterpolatorModifierSelector" , m_interpolatorModifier))
	{
		m_interpolatorModifier->Init();
	}

	if (!m_interpolatorModifier) { return; }
	m_interpolatorModifier->EditPrefabInspector();
}

void MRI::ComponentMode::RotationComponentSmoothModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::ComponentMode::RotationComponentModeBase::DeserializePrefab					 (a_json);
	MRI::JsonUtility::DeserializeInstancePrefab<MRI::SharedFactory::InterpolatorModifier>(a_json , "InterpolatorModifierName" , m_interpolatorModifier);
}

nlohmann::json MRI::ComponentMode::RotationComponentSmoothModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	if (!m_interpolatorModifier)
	{
		return nlohmann::json();
	}

	MRI::JsonUtility::UpdateJson(l_rootJson, MRI::ComponentMode::RotationComponentModeBase::SerializePrefab());
	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::JsonUtility::SerializeInstancePrefab					   ("InterpolatorModifierName" , m_interpolatorModifier));

	return l_rootJson;
}