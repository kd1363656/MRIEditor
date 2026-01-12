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

	const auto& l_factory = MRI::SharedFactory::InterpolatorModifier::GetInstance();

	const std::string l_interpolatorModifierBaseName = a_json.value    ("InterpolatorModifierBaseName" , std::string());
	m_interpolatorModifierBase						 = l_factory.Create(l_interpolatorModifierBaseName);

	if (m_interpolatorModifierBase)
	{
		m_interpolatorModifierBase->DeserializePrefab(a_json);
	}
}

nlohmann::json MRI::ComponentMode::MoveComponentLerpModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	if (m_interpolatorModifierBase) 
	{
		l_rootJson["InterpolatorModifierBaseName"] = m_interpolatorModifierBase->GetTypeInfo().k_name.data();
		MRI::JsonUtility::UpdateJson                                                    (l_rootJson , m_interpolatorModifierBase->SerializePrefab());
	}

	return l_rootJson;
}