#include "MRIRotationComponentInstantByMouseMode.h"

#include "Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentInstantByMouseMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentInstantByMouseMode>();
}

void MRI::ComponentMode::RotationComponentInstantByMouseMode::Init()
{
	MRI::ComponentMode::RotationComponentInstantModeBase::Init();

	if (!m_rotationComponentByMouseHelper)
	{
		m_rotationComponentByMouseHelper = std::make_shared<MRI::Helper::RotationComponentByMouseHelper>();
	}

	m_rotationComponentByMouseHelper->Init();
}

void MRI::ComponentMode::RotationComponentInstantByMouseMode::Update()
{
	auto l_selfTransformComponentCache = MRI::ComponentMode::RotationComponentModeBase::GetWorkSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	if (!m_rotationComponentByMouseHelper) { return; }

	// マウスによる回転を有効にするためにマウスを画面中央に固定するかどうか
	m_rotationComponentByMouseHelper->ToggleMouseCenterLock();

	// もしマウスロックが解除されていたら実行しない
	if (m_rotationComponentByMouseHelper->GetIsDisableMouseCenterLock()) { return; }

	const auto& l_application  = Application::GetInstance      ();
	const auto& l_inputManager = MRI::InputManager::GetInstance();

	// マウスの移動量を取得
	const Math::Vector3& l_mouseMovement = l_inputManager.FetchMouseDeltaAndResetCursorCenter();
	
	// マウスの移動量がほとんどないなら"return"
	if (l_mouseMovement.LengthSquared() <= CommonConstant::k_epsilon) { return; }

	// "X"と"Y"の値を入れ替える("Y"方向の移動は"X","X"方向の移動は"Y"として書いては扱うから)
	const Math::Vector3& l_movement = { l_mouseMovement.y , l_mouseMovement.x , l_mouseMovement.z };

	// 回転速度を取得
	const float l_rotationSpeed = MRI::ComponentMode::RotationComponentInstantModeBase::GetRotationSpeed() * l_application.GetScaledDeltaTime();

	// ターゲット回転からオイラー角を取得
	const Math::Vector3& l_rotation      = MRI::ComponentMode::RotationComponentInstantModeBase::GetRotationDirection();
	Math::Vector3        l_putOnRotation = l_rotation * l_rotationSpeed;

	// 許可された方向への回転方向の加算をする
	MRI::AxisUtility::AddAdaptDirection(MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationDirectionTagList() , l_putOnRotation , l_movement);

	// "X"軸の回転にだけ制限を掛ける
	l_putOnRotation.x = std::clamp(l_putOnRotation.x , m_rotationComponentByMouseHelper->GetMinRotatableDegreeX() , m_rotationComponentByMouseHelper->GetMaxRotatableDegreeX());
	MRI::ComponentMode::RotationComponentInstantModeBase::SetRotationDirection								   (l_putOnRotation);

	const Math::Quaternion& l_resultRotation = MRI::MathUtility::EulerToQuaternion(MRI::ComponentMode::RotationComponentInstantModeBase::GetRotationDirection());
	l_selfTransformComponentCache->SetRotation(l_resultRotation);
}

void MRI::ComponentMode::RotationComponentInstantByMouseMode::EditPrefabInspector()
{
	MRI::ComponentMode::RotationComponentInstantModeBase::EditPrefabInspector();

	if (m_rotationComponentByMouseHelper)
	{
		m_rotationComponentByMouseHelper->EditPrefabInspector();
	}
}

void MRI::ComponentMode::RotationComponentInstantByMouseMode::DeserializePrefab(const nlohmann::json& a_json)
{
	MRI::ComponentMode::RotationComponentInstantModeBase::DeserializePrefab(a_json);

	if (m_rotationComponentByMouseHelper)
	{
		m_rotationComponentByMouseHelper->DeserializePrefab(a_json);
	}
}

nlohmann::json MRI::ComponentMode::RotationComponentInstantByMouseMode::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::ComponentMode::RotationComponentInstantModeBase::SerializePrefab());

	if (m_rotationComponentByMouseHelper)
	{
		MRI::JsonUtility::UpdateJson(l_rootJson , m_rotationComponentByMouseHelper->SerializePrefab());
	}

	return l_rootJson;
}
