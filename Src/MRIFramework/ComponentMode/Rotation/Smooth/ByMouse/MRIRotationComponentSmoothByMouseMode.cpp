#include "MRIRotationComponentSmoothByMouseMode.h"

#include "Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentSmoothByMouseMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentSmoothByMouseMode>();
}

void MRI::ComponentMode::RotationComponentSmoothByMouseMode::Init()
{
	MRI::ComponentMode::RotationComponentSmoothModeBase::Init();

	if (!m_rotationComponentByMouseHelper)
	{
		m_rotationComponentByMouseHelper = std::make_shared<MRI::Helper::RotationComponentByMouseHelper>();
	}

	m_rotationComponentByMouseHelper->Init();
}

void MRI::ComponentMode::RotationComponentSmoothByMouseMode::Update()
{
	auto l_selfTransformComponentCache = MRI::ComponentMode::RotationComponentModeBase::GetWorkSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	auto l_interpolatorModifierCache = MRI::ComponentMode::RotationComponentSmoothModeBase::GetInterpolatorModifierCache().lock();
	if (!l_interpolatorModifierCache) { return; }

	if (!m_rotationComponentByMouseHelper) { return; }

	// マウスによる回転を有効にするためにマウスを画面中央に固定するかどうか
	m_rotationComponentByMouseHelper->ToggleMouseCenterLock();

	// もしマウスロックが解除されていたら実行しない
	if (m_rotationComponentByMouseHelper->GetIsDisableMouseCenterLock()) { return; }

	const auto& l_application  = Application::GetInstance      ();
	const auto& l_inputManager = MRI::InputManager::GetInstance();

	// マウスの移動量を取得
	const Math::Vector3& l_mouseMovement = l_inputManager.FetchMouseDeltaAndResetCursorCenter();
	
	// マウスの移動量がほとんどないなら補完をリセットして"return"
	if (l_mouseMovement.LengthSquared() <= CommonConstant::k_epsilon) 
	{ 
		l_interpolatorModifierCache->ResetInterpolate();
		return;
	}

	// "X"と"Y"の値を入れ替える("Y"方向の回転は"X","X"方向の回転は"Y"として扱うから)
	Math::Vector3 l_movement = { l_mouseMovement.y , l_mouseMovement.x , l_mouseMovement.z };

	const float l_deltaTime = l_application.GetScaledDeltaTime();

	// 回転速度にデルタタイムを乗算
	l_movement *= l_interpolatorModifierCache->GetCurrentValue() * l_deltaTime;

	// 現在のオイラー角を取得
	Math::Vector3 l_rotation = MRI::ComponentMode::RotationComponentModeBase::GetRotationDirection();

	// 回転適用が許されている軸にのみ回転を加算
	MRI::AxisUtility::AddAdaptDirection(MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationDirectionTagList() , l_rotation , l_movement);

	// "X"軸の回転にのみ制限を掛ける(必要なら"3"軸全てにかけれるようにする)
	l_rotation.x = std::clamp(l_rotation.x , m_rotationComponentByMouseHelper->GetMinRotatableDegreeX() , m_rotationComponentByMouseHelper->GetMaxRotatableDegreeX());

	// 回転方向を格納
	MRI::ComponentMode::RotationComponentModeBase::SetRotationDirection(l_rotation);

	const Math::Quaternion& l_currentRotation = l_selfTransformComponentCache->GetRotation();
	Math::Quaternion        l_targetRotation  = MRI::MathUtility::EulerToQuaternion       (MRI::ComponentMode::RotationComponentModeBase::GetRotationDirection());

	// クオータニオン補正(最短経路を決める)
	if (l_currentRotation.Dot(l_targetRotation) <= MRI::CommonConstant::k_epsilon)
	{
		l_targetRotation = -l_targetRotation;
	}

	// カメラの回転方向を格納
	Math::Quaternion l_resultRotation = Math::Quaternion::Slerp(l_currentRotation , l_targetRotation , l_interpolatorModifierCache->GetCurrentValue() * l_deltaTime);
	l_selfTransformComponentCache->SetRotation				   (l_resultRotation);

	// 最後に補完の進捗を進める
	l_interpolatorModifierCache->Update();
}

void MRI::ComponentMode::RotationComponentSmoothByMouseMode::EditPrefabInspector()
{
	MRI::ComponentMode::RotationComponentSmoothModeBase::EditPrefabInspector();

	if (m_rotationComponentByMouseHelper)
	{
		m_rotationComponentByMouseHelper->EditPrefabInspector();
	}
}

void MRI::ComponentMode::RotationComponentSmoothByMouseMode::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::ComponentMode::RotationComponentSmoothModeBase::DeserializePrefab(a_json);

	if (m_rotationComponentByMouseHelper)
	{
		m_rotationComponentByMouseHelper->DeserializePrefab(a_json);
	}
}

nlohmann::json MRI::ComponentMode::RotationComponentSmoothByMouseMode::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::ComponentMode::RotationComponentSmoothModeBase::SerializePrefab());
	
	if (m_rotationComponentByMouseHelper)
	{
		MRI::JsonUtility::UpdateJson(l_rootJson , m_rotationComponentByMouseHelper->SerializePrefab());
	}

	return l_rootJson;
}