#include "MRIMoveComponentLerpModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::MoveComponentLerpModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentLerpModeBase>();
}

void MRI::ComponentMode::MoveComponentLerpModeBase::EditPrefabInspector()
{
	// 補完クラスセレクター
	MRI::EditorUtility::FactoryRadioButtonSelector<MRI::SharedFactory::InterpolatorModifier>("InterpolatorSelector" , m_interpolatorModifierBase);

	if (!m_interpolatorModifierBase) { return; }
	m_interpolatorModifierBase->EditPrefabInspector();
}

void MRI::ComponentMode::MoveComponentLerpModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::JsonUtility::DeserializeInstancePrefab<MRI::SharedFactory::InterpolatorModifier>(a_json , "InterpolatorModifierBaseName" , m_interpolatorModifierBase);
}

nlohmann::json MRI::ComponentMode::MoveComponentLerpModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	if (!m_interpolatorModifierBase) 
	{
		return nlohmann::json();
	}

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::JsonUtility::SerializeInstancePrefab("InterpolatorModifierBaseName" , m_interpolatorModifierBase));

	return l_rootJson;
}