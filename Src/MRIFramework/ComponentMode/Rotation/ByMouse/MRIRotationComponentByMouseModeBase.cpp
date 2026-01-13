#include "MRIRotationComponentByMouseModeBase.h"

#include "Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentByMouseModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentByMouseModeBase>();
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::Init()
{
	m_rotationSpeed = k_defaultRotationSpeed;

	m_minRotatableDegreeX = -MRI::CommonConstant::k_quarterDegree;
	m_maxRotatableDegreeX =  MRI::CommonConstant::k_quarterDegree;

	m_isDisableMouseCenterLock = false;
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::Update()
{
	// マウスの移動を中心に固定するかどうか
	ToggleMouseCenterLock();

	if (MRI::ComponentMode::RotationComponentByMouseModeBase::GetIsDisableMouseCenterLock()) { return; }
	
	const auto& l_application  = Application::GetInstance        ();
	const auto& l_inputManager = MRI::InputManager::GetInstance  ();
	const float l_deltaTime    = l_application.GetScaledDeltaTime();

	const Math::Vector3& l_mouseMovement = l_inputManager.FetchMouseDeltaAndResetCursorCenter();
	Math::Vector3        l_movement      = { l_mouseMovement.y , l_mouseMovement.x , l_mouseMovement.z};

	// マウスの移動量がほとんどないなら補完の進捗をリセットして"return"
	if (l_movement.LengthSquared() <= CommonConstant::k_epsilon) { return; }
	
	// 移動量に回転速度を掛ける
	l_movement *= m_rotationSpeed * l_deltaTime;

	// ターゲット回転からオイラー角を取得
	Math::Vector3 l_rotation = MRI::ComponentMode::RotationComponentByMouseModeBase::GetRotationDirection();

	// 回転適用が許されている軸にのみ回転を加算
	MRI::AxisUtility::AddAdaptDirection(MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationDirectionTagList() , l_rotation , l_movement);

	// 向くべき方向を格納
	MRI::ComponentMode::RotationComponentModeBase::SetRotationDirection(l_rotation);	
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::EditPrefabInspector()
{
	ImGui::DragFloat("MinRotatableDegreeX"   , &m_minRotatableDegreeX , MRI::EditorCommonConstant::k_defaultDragValue);
	ImGui::DragFloat("RotationSpeed"         , &m_rotationSpeed       , MRI::EditorCommonConstant::k_defaultDragValue);
	ImGui::DragFloat("MaxRotatableDegreeX"   , &m_maxRotatableDegreeX , MRI::EditorCommonConstant::k_defaultDragValue);
	ImGui::Text     ("DisableMouseLock : %s" , MRI::EditorUtility::BoolToString(m_isDisableMouseCenterLock));

	m_minRotatableDegreeX = std::clamp(m_minRotatableDegreeX , -MRI::CommonConstant::k_quarterDegree , 0.0F);
	m_maxRotatableDegreeX = std::clamp(m_maxRotatableDegreeX , 0.0F                                  , MRI::CommonConstant::k_quarterDegree);
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	m_rotationSpeed       = a_json.value("RotationSpeed"       ,  k_defaultRotationSpeed);
	m_minRotatableDegreeX = a_json.value("MinRotatableDegreeX" , -MRI::CommonConstant::k_quarterDegree);
	m_maxRotatableDegreeX = a_json.value("MaxRotatableDegreeX" ,  MRI::CommonConstant::k_quarterDegree);
}

nlohmann::json MRI::ComponentMode::RotationComponentByMouseModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	l_rootJson["RotationSpeed"]       = m_rotationSpeed;
	l_rootJson["MinRotatableDegreeX"] = m_minRotatableDegreeX;
	l_rootJson["MaxRotatableDegreeX"] = m_maxRotatableDegreeX;

	return l_rootJson;
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::ToggleMouseCenterLock()
{
	// 一回だけキー入力に反応してトグル操作を行う
	if (const auto& l_input = MRI::InputManager::GetInstance();
		l_input.IsInputOnce(VK_TAB))
	{
		m_isDisableMouseCenterLock = m_isDisableMouseCenterLock ? false : true;
	}
}