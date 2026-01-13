#include "MRIRotationComponentInstantModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentInstantModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentInstantModeBase>();
}

void MRI::ComponentMode::RotationComponentInstantModeBase::Init()
{
	MRI::ComponentMode::RotationComponentModeBase::Init();

	m_rotationSpeed = k_defaultRotationSpeed;
}

void MRI::ComponentMode::RotationComponentInstantModeBase::EditPrefabInspector()
{
	MRI::ComponentMode::RotationComponentModeBase::EditPrefabInspector();

	ImGui::DragFloat("RotationSpeed" , &m_rotationSpeed , MRI::EditorCommonConstant::k_defaultDragValue);
}

void MRI::ComponentMode::RotationComponentInstantModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::ComponentMode::RotationComponentModeBase::DeserializePrefab(a_json);

	m_rotationSpeed = a_json.value("RotationSpeed" , k_defaultRotationSpeed);
}

nlohmann::json MRI::ComponentMode::RotationComponentInstantModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::ComponentMode::RotationComponentModeBase::SerializePrefab());

	l_rootJson["RotationSpeed"] = m_rotationSpeed;

	return l_rootJson;
}