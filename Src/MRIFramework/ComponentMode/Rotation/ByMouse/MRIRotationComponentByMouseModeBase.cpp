#include "MRIRotationComponentByMouseModeBase.h"

#include "Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentByMouseModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentByMouseModeBase>();
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::Init()
{
	MRI::ComponentMode::RotationComponentModeBase::Init();

	m_rotationSpeed = k_defaultRotationSpeed;

	m_minRotatableDegreeX = -MRI::CommonConstant::k_quarterDegree;
	m_maxRotatableDegreeX =  MRI::CommonConstant::k_quarterDegree;

	m_isDisableMouseCenterLock = false;
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::Update()
{
	auto l_selfTransformComponentCache = MRI::ComponentMode::RotationComponentModeBase::GetWorkSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	// マウスによる回転を有効にするためにマウスを画面中央に固定するかどうか
	ToggleMouseCenterLock();

	// もしマウスロックが解除されていたら実行しない
	if (m_isDisableMouseCenterLock) { return; }

	const auto& l_application  = Application::GetInstance      ();
	const auto& l_inputManager = MRI::InputManager::GetInstance();

	// マウスの移動量を取得
	const Math::Vector3& l_mouseMovement = l_inputManager.FetchMouseDeltaAndResetCursorCenter();
	
	// マウスの移動量がほとんどないなら"return"
	if (l_mouseMovement.LengthSquared() <= CommonConstant::k_epsilon) { return; }

	// "X"と"Y"の値を入れ替える("Y"方向の回転は"X","X"方向の回転は"Y"として扱うから)
	Math::Vector3 l_movement = { l_mouseMovement.y , l_mouseMovement.x , l_mouseMovement.z };

	// 回転速度にデルタタイムを乗算
	l_movement *= MRI::ComponentMode::RotationComponentByMouseModeBase::GetRotationSpeed() * l_application.GetScaledDeltaTime();

	// 現在のオイラー角を取得
	Math::Vector3 l_rotation = MRI::ComponentMode::RotationComponentModeBase::GetRotationDirection();

	// 回転適用が許されている軸にのみ回転を加算
	MRI::AxisUtility::AddAdaptDirection(MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationDirectionTagList() , l_rotation , l_movement);

	// "X"軸の回転にのみ制限を掛ける(必要なら"3"軸全てにかけれるようにする)
	l_rotation.x = std::clamp(l_rotation.x , MRI::ComponentMode::RotationComponentByMouseModeBase::GetMinRotatableDegreeX() , MRI::ComponentMode::RotationComponentByMouseModeBase::GetMaxRotatableDegreeX());

	// 回転方向を格納
	MRI::ComponentMode::RotationComponentModeBase::SetRotationDirection(l_rotation);

	// オイラー角からクオータニオンに変換して"TransformComponent"に格納
	const auto& l_resultRotation = MRI::MathUtility::EulerToQuaternion(MRI::ComponentMode::RotationComponentModeBase::GetRotationDirection());
	l_selfTransformComponentCache->SetRotation						  (l_resultRotation);
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::EditPrefabInspector()
{
	MRI::ComponentMode::RotationComponentModeBase::EditPrefabInspector();

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

	MRI::ComponentMode::RotationComponentModeBase::DeserializePrefab(a_json);

	m_rotationSpeed       = a_json.value("RotationSpeed"       ,  k_defaultRotationSpeed);
	m_minRotatableDegreeX = a_json.value("MinRotatableDegreeX" , -MRI::CommonConstant::k_quarterDegree);
	m_maxRotatableDegreeX = a_json.value("MaxRotatableDegreeX" ,  MRI::CommonConstant::k_quarterDegree);
}

nlohmann::json MRI::ComponentMode::RotationComponentByMouseModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::ComponentMode::RotationComponentModeBase::SerializePrefab());

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