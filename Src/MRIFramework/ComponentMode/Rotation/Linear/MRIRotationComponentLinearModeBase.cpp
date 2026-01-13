#include "MRIRotationComponentLinearModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentLinearModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentLinearModeBase>();
}

void MRI::ComponentMode::RotationComponentLinearModeBase::Init()
{
	MRI::ComponentMode::RotationComponentModeBase::Init();

	m_rotationSpeed = k_defaultRotationSpeed;
}

void MRI::ComponentMode::RotationComponentLinearModeBase::EditPrefabInspector()
{
	MRI::ComponentMode::RotationComponentModeBase::EditPrefabInspector();

	ImGui::DragFloat("RotationSpeed" , &m_rotationSpeed , MRI::EditorCommonConstant::k_defaultDragValue);
}

void MRI::ComponentMode::RotationComponentLinearModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::ComponentMode::RotationComponentModeBase::DeserializePrefab(a_json);

	m_rotationSpeed = a_json.value("RotationSpeed" , k_defaultRotationSpeed);
}

nlohmann::json MRI::ComponentMode::RotationComponentLinearModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::ComponentMode::RotationComponentModeBase::SerializePrefab());

	l_rootJson["RotationSpeed"] = m_rotationSpeed;

	return l_rootJson;
}