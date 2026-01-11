#include "MRIMoveComponentLerpMode.h"

const MRI::TypeInfo& MRI::ComponentMode::MoveComponentLerpMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentLerpMode>();
}

void MRI::ComponentMode::MoveComponentLerpMode::EditPrefabInspector()
{
	// 基底クラスのプレハブインスペクター
	MRI::ComponentMode::MoveComponentLerpMode::EditPrefabInspector();

	// 補完クラスセレクター
	MRI::EditorUtility::FactoryRadioButtonSelector<MRI::SharedFactory::InterpolatorModifier>("InterpolatorModifierSelector", m_interpolatorModifier);

	if (m_interpolatorModifier)
	{
		m_interpolatorModifier->EditPrefabInspector();
	}
}

void MRI::ComponentMode::MoveComponentLerpMode::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::ComponentMode::MoveComponentModeBase::DeserializePrefab(a_json);

	const auto& l_factory = MRI::SharedFactory::InterpolatorModifier::GetInstance();

	const std::string& l_interpolatorModifierName = a_json.value("InterpolatorModifierName", std::string());

	m_interpolatorModifier = l_factory.Create(l_interpolatorModifierName);

	if (m_interpolatorModifier)
	{
		m_interpolatorModifier->DeserializePrefab(a_json);
	}
}

nlohmann::json MRI::ComponentMode::MoveComponentLerpMode::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::ComponentMode::MoveComponentModeBase::SerializePrefab());

	if (m_interpolatorModifier)
	{
		l_rootJson["InterpolatorModifierName"] = m_interpolatorModifier->GetTypeInfo().k_name.data();
		MRI::JsonUtility::UpdateJson(l_rootJson, m_interpolatorModifier->SerializePrefab());
	}

	return l_rootJson;
}