#include "MRIMoveComponentLinearModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::MoveComponentLinearModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentLinearModeBase>();
}

void MRI::ComponentMode::MoveComponentLinearModeBase::Init()
{
	MRI::ComponentMode::MoveComponentModeBase::Init();

	m_moveSpeed = k_defaultMoveSpeed;
}

void MRI::ComponentMode::MoveComponentLinearModeBase::EditPrefabInspector()
{
	MRI::ComponentMode::MoveComponentModeBase::EditPrefabInspector();

	ImGui::DragFloat("MoveSpeed" , &m_moveSpeed , MRI::EditorCommonConstant::k_defaultDragValue);
}

void MRI::ComponentMode::MoveComponentLinearModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::ComponentMode::MoveComponentModeBase::DeserializePrefab(a_json);

	m_moveSpeed = a_json.value("MoveSpeed" , k_defaultMoveSpeed);
}

nlohmann::json MRI::ComponentMode::MoveComponentLinearModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::ComponentMode::MoveComponentModeBase::SerializePrefab());

	l_rootJson["MoveSpeed"] = m_moveSpeed;

	return l_rootJson;
}