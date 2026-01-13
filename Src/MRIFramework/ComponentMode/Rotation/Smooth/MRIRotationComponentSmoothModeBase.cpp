#include "MRIRotationComponentSmoothModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentSmoothModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentSmoothModeBase>();
}

void MRI::ComponentMode::RotationComponentSmoothModeBase::EditPrefabInspector()
{
	MRI::ComponentMode::RotationComponentModeBase::EditPrefabInspector();

	MRI::EditorUtility::FactoryRadioButtonSelector<MRI::SharedFactory::InterpolatorModifier>("InterpolatorModifierSelector" , m_interpolatorModifier);

	if (!m_interpolatorModifier) { return; }
	m_interpolatorModifier->EditPrefabInspector();
}

void MRI::ComponentMode::RotationComponentSmoothModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::ComponentMode::RotationComponentModeBase::DeserializePrefab(a_json);

	const auto& l_sharedFactory = MRI::SharedFactory::InterpolatorModifier::GetInstance();

	const std::string& l_interpolatorModifierName = a_json.value          ("InterpolatorModifierName" , std::string());
	m_interpolatorModifier						  = l_sharedFactory.Create(l_interpolatorModifierName);

	if (!m_interpolatorModifier) { return; }
	m_interpolatorModifier->Init             ();
	m_interpolatorModifier->DeserializePrefab(a_json);
}

nlohmann::json MRI::ComponentMode::RotationComponentSmoothModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson, MRI::ComponentMode::RotationComponentModeBase::SerializePrefab());

	if (m_interpolatorModifier)
	{
		l_rootJson["InterpolatorModifierName"] = m_interpolatorModifier->GetTypeInfo().k_name.data();
		MRI::JsonUtility::UpdateJson                                                (l_rootJson , m_interpolatorModifier->SerializePrefab());
	}
	
	return l_rootJson;
}