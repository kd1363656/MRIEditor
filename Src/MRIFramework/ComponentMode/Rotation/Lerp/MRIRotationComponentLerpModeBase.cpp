#include "MRIRotationComponentLerpModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentLerpModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentLerpModeBase>();
}

void MRI::ComponentMode::RotationComponentLerpModeBase::EditPrefabInspector()
{
	MRI::ComponentMode::RotationComponentModeBase::EditPrefabInspector();

	// 作製されたら初期化のみ行う
	if (MRI::EditorUtility::FactoryRadioButtonSelector<MRI::SharedFactory::InterpolatorModifier>("InterpolatorModifierSelector" , m_interpolatorModifier) && m_interpolatorModifier)
	{
		m_interpolatorModifier->Init();
	}

	if (!m_interpolatorModifier) { return; }
	m_interpolatorModifier->EditPrefabInspector();
}

void MRI::ComponentMode::RotationComponentLerpModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::ComponentMode::RotationComponentModeBase::DeserializePrefab(a_json);

	MRI::JsonUtility::DeserializeInstancePrefab<MRI::SharedFactory::InterpolatorModifier>(a_json , "InterpolatorModifierName" , m_interpolatorModifier);
}

nlohmann::json MRI::ComponentMode::RotationComponentLerpModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	if (!m_interpolatorModifier)
	{
		return nlohmann::json();
	}

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::ComponentMode::RotationComponentModeBase::SerializePrefab());
	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::JsonUtility::SerializeInstancePrefab					    ("InterpolatorModifierName" , m_interpolatorModifier));

	return l_rootJson;
}