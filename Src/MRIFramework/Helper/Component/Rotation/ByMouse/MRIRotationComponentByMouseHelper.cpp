#include "MRIRotationComponentByMouseHelper.h"

void MRI::Helper::RotationComponentByMouseHelper::Init()
{
	m_minRotatableDegreeX = -MRI::CommonConstant::k_quarterDegree;
	m_maxRotatableDegreeX =  MRI::CommonConstant::k_quarterDegree;

	m_isDisableMouseCenterLock = false;
}

void MRI::Helper::RotationComponentByMouseHelper::ToggleMouseCenterLock()
{
	// 一回だけキー入力に反応してトグル操作を行う
	if (const auto& l_input = MRI::InputManager::GetInstance();
		l_input.IsInputOnce(VK_TAB))
	{
		m_isDisableMouseCenterLock = m_isDisableMouseCenterLock ? false : true;
	}
}

void MRI::Helper::RotationComponentByMouseHelper::EditPrefabInspector()
{
	ImGui::DragFloat("MinRotatableDegreeX"   , &m_minRotatableDegreeX , MRI::EditorCommonConstant::k_defaultDragValue);
	ImGui::DragFloat("MaxRotatableDegreeX"   , &m_maxRotatableDegreeX , MRI::EditorCommonConstant::k_defaultDragValue);
	ImGui::Text     ("DisableMouseLock : %s" , MRI::EditorUtility::BoolToString(m_isDisableMouseCenterLock));

	m_minRotatableDegreeX = std::clamp(m_minRotatableDegreeX , -MRI::CommonConstant::k_quarterDegree , 0.0F);
	m_maxRotatableDegreeX = std::clamp(m_maxRotatableDegreeX , 0.0F                                  , MRI::CommonConstant::k_quarterDegree);
}

void MRI::Helper::RotationComponentByMouseHelper::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	a_json.value("MinRotatableDegreeX" , -MRI::CommonConstant::k_quarterDegree);
	a_json.value("MaxRotatableDegreeX" ,  MRI::CommonConstant::k_quarterDegree);
}

nlohmann::json MRI::Helper::RotationComponentByMouseHelper::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	l_rootJson["MinRotatableDegreeX"] = m_minRotatableDegreeX;
	l_rootJson["MaxRotatableDegreeX"] = m_maxRotatableDegreeX;

	return l_rootJson;
}