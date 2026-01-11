#include "MRIInterpolatorModifierBase.h"

const MRI::TypeInfo& MRI::Modifier::InterpolatorModifierBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Modifier::InterpolatorModifierBase>();
}

void MRI::Modifier::InterpolatorModifierBase::Init()
{
	m_progress = 0.0F;

	m_maxMeasurementTime = k_maxMeasurementTime;
	m_elapsedTime        = 0.0f;

	m_maxValue     = k_maxClampValue;
	m_currentValue = 0.0f;
}

void MRI::Modifier::InterpolatorModifierBase::EditPrefabInspector()
{
	ImGui::Text("Progress     : %.2f" , m_progress);
	ImGui::Text("ElapsedTime  : %.2f" , m_elapsedTime);
	ImGui::Text("CurrentValue : %.2f" , m_currentValue);

	ImGui::DragFloat("MaxMeasurementTime" , &m_maxMeasurementTime , MRI::EditorCommonConstant::k_defaultDragValue);
	ImGui::DragFloat("MaxValue"           , &m_maxValue           , MRI::EditorCommonConstant::k_defaultDragValue);

	// 各種パラメータの最大値が"0"を下回らないようにする
	m_maxMeasurementTime = std::max(m_maxMeasurementTime , k_maxMeasurementTime);
	m_elapsedTime        = std::max(m_maxValue           , k_maxClampValue);
}

void MRI::Modifier::InterpolatorModifierBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	m_maxMeasurementTime = a_json.value("MaxMeasurementTime" , 0.0f);
	m_maxValue           = a_json.value("MaxValue"           , k_maxClampValue);
}
nlohmann::json MRI::Modifier::InterpolatorModifierBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	l_rootJson["MaxMeasurementTime"] = m_maxMeasurementTime;
	l_rootJson["MaxValue"]           = m_maxValue;

	return l_rootJson;
}

void MRI::Modifier::InterpolatorModifierBase::ResetInterpolate()
{
	m_progress = 0.0F;

	m_elapsedTime = 0.0F;

	m_currentValue = 0.0F;
}