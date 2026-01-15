#include "MRIRotationComponentConstantSpeedMouseMode.h"

#include "../../../../../Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentConstantSpeedMouseMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentConstantSpeedMouseMode>();
}

void MRI::ComponentMode::RotationComponentConstantSpeedMouseMode::Init()
{
	MRI::ComponentMode::RotationComponentConstantSpeedModeBase::Init();

	if (!m_componentRotationByMouseHelper)
	{
		m_componentRotationByMouseHelper = std::make_shared<MRI::Helper::ComponentRotationByMouseHelper>();
	}

	m_componentRotationByMouseHelper->Init();
}

void MRI::ComponentMode::RotationComponentConstantSpeedMouseMode::Update()
{
	if (!m_componentRotationByMouseHelper) { return; }

	auto l_selfTransformComponentCache = MRI::ComponentMode::RotationComponentModeBase::GetWorkSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	// マウスロックするかどうかを切り替える
	m_componentRotationByMouseHelper->ToggleMouseCenterLock();

	// もしマウスロックがされていなければ"return"
	if (m_componentRotationByMouseHelper->GetIsDisableMouseCenterLock()) { return; }

	const auto& l_application  = Application::GetInstance      ();
	const auto& l_inputManager = MRI::InputManager::GetInstance();

	// マウスの移動量を取得
	const Math::Vector3& l_mouseMovement = l_inputManager.FetchMouseDeltaAndResetCursorCenter();
	
	// マウスの移動量がほとんどないなら"return"
	if (l_mouseMovement.LengthSquared() <= CommonConstant::k_epsilon) { return; }

	// "X"と"Y"の値を入れ替える("Y"方向の回転は"X","X"方向の回転は"Y"として扱うから)
	Math::Vector3 l_movement = { l_mouseMovement.y , l_mouseMovement.x , l_mouseMovement.z };

	// 回転速度にデルタタイムを乗算
	l_movement *= MRI::ComponentMode::RotationComponentConstantSpeedModeBase::GetRotationSpeed() * l_application.GetScaledDeltaTime();

	// 現在のオイラー角を取得
	Math::Vector3 l_rotation = MRI::ComponentMode::RotationComponentModeBase::GetRotationDirection();

	// 回転適用が許されている軸にのみ回転を加算
	MRI::AxisUtility::AddAdaptAxisValue(MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationAxisTagSet() , l_rotation , l_movement);
	MRI::AxisUtility::ResetUnusedAxis  (MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationAxisTagSet() , l_rotation);

	// "X"軸の回転にのみ制限を掛ける(必要なら"3"軸全てにかけれるようにする)
	l_rotation.x = std::clamp(l_rotation.x , m_componentRotationByMouseHelper->GetMinRotatableDegreeX() , m_componentRotationByMouseHelper->GetMaxRotatableDegreeX());

	// 回転方向を格納
	MRI::ComponentMode::RotationComponentModeBase::SetRotationDirection(l_rotation);

	// オイラー角からクオータニオンに変換して"TransformComponent"に格納
	const auto& l_resultRotation = MRI::MathUtility::EulerToQuaternion(MRI::ComponentMode::RotationComponentModeBase::GetRotationDirection());
	l_selfTransformComponentCache->SetRotation						  (l_resultRotation);
}

void MRI::ComponentMode::RotationComponentConstantSpeedMouseMode::EditPrefabInspector()
{
	MRI::ComponentMode::RotationComponentConstantSpeedModeBase::EditPrefabInspector();

	if (!m_componentRotationByMouseHelper) { return; }
	
	m_componentRotationByMouseHelper->EditPrefabInspector();
}

void MRI::ComponentMode::RotationComponentConstantSpeedMouseMode::DeserializePrefab(const nlohmann::json& a_json)
{
	MRI::ComponentMode::RotationComponentConstantSpeedModeBase::RotationComponentConstantSpeedModeBase::DeserializePrefab(a_json);

	if (!m_componentRotationByMouseHelper) { return; }

	m_componentRotationByMouseHelper->DeserializePrefab(a_json);
}

nlohmann::json MRI::ComponentMode::RotationComponentConstantSpeedMouseMode::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	if (!m_componentRotationByMouseHelper) 
	{
		return nlohmann::json();
	}

	MRI::JsonUtility::UpdateJson(l_rootJson , m_componentRotationByMouseHelper->SerializePrefab					         ());
	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::ComponentMode::RotationComponentConstantSpeedModeBase::SerializePrefab());

	return l_rootJson;
}