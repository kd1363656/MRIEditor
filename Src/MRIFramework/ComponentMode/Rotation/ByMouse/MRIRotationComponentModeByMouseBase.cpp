#include "MRIRotationComponentModeByMouseBase.h"

void MRI::ComponentMode::RotationComponentModeByMouseBase::Init()
{
	m_rotationSpeed = k_defaultRotationSpeed;

	m_minRotatableDegreeX = -MRI::CommonConstant::k_quarterDegree;
	m_maxRotatableDegreeX =  MRI::CommonConstant::k_quarterDegree;

	m_isDisableMouseCenterLock = false;
}

void MRI::ComponentMode::RotationComponentModeByMouseBase::EditPrefabInspector()
{
	ImGui::DragFloat("MinRotatableDegreeX"   , &m_minRotatableDegreeX , MRI::EditorCommonConstant::k_defaultDragValue);
	ImGui::DragFloat("RotationSpeed"         , &m_rotationSpeed       , MRI::EditorCommonConstant::k_defaultDragValue);
	ImGui::DragFloat("MaxRotatableDegreeX"   , &m_maxRotatableDegreeX , MRI::EditorCommonConstant::k_defaultDragValue);
	ImGui::Text     ("DisableMouseLock : %s" , MRI::EditorUtility::BoolToString(m_isDisableMouseCenterLock));

	m_minRotatableDegreeX = std::clamp(m_minRotatableDegreeX , -MRI::CommonConstant::k_quarterDegree , 0.0F);
	m_maxRotatableDegreeX = std::clamp(m_maxRotatableDegreeX , 0.0F                                  , MRI::CommonConstant::k_quarterDegree);
}

void MRI::ComponentMode::RotationComponentModeByMouseBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	m_rotationSpeed       = a_json.value("RotationSpeed"       ,  k_defaultRotationSpeed);
	m_minRotatableDegreeX = a_json.value("MinRotatableDegreeX" , -MRI::CommonConstant::k_quarterDegree);
	m_maxRotatableDegreeX = a_json.value("MaxRotatableDegreeX" ,  MRI::CommonConstant::k_quarterDegree);
}

nlohmann::json MRI::ComponentMode::RotationComponentModeByMouseBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	l_rootJson["RotationSpeed"]       = m_rotationSpeed;
	l_rootJson["MinRotatableDegreeX"] = m_minRotatableDegreeX;
	l_rootJson["MaxRotatableDegreeX"] = m_maxRotatableDegreeX;

	return l_rootJson;
}

void MRI::ComponentMode::RotationComponentModeByMouseBase::ToggleMouseCenterLock()
{
	// 一回だけキー入力に反応してトグル操作を行う
	if (const auto& l_input = MRI::InputManager::GetInstance();
		l_input.IsInputOnce(VK_TAB))
	{
		m_isDisableMouseCenterLock = m_isDisableMouseCenterLock ? false : true;
	}
}