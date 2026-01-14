#include "MRIRotationComponentLerpMouseMode.h"

#include "../../../../../Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentLerpMouseMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentLerpMouseMode>();
}

void MRI::ComponentMode::RotationComponentLerpMouseMode::Init()
{
	MRI::ComponentMode::RotationComponentLerpModeBase::Init();

	if (!m_componentRotationByMouseHelper)
	{
		m_componentRotationByMouseHelper = std::make_shared<MRI::Helper::ComponentRotationByMouseHelper>();
	}

	m_componentRotationByMouseHelper->Init();
}

void MRI::ComponentMode::RotationComponentLerpMouseMode::Update()
{
	if (!m_componentRotationByMouseHelper) { return; }

	auto l_selfTransformComponentCache = MRI::ComponentMode::RotationComponentModeBase::GetWorkSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	auto l_interpolatorModifierCache = MRI::ComponentMode::RotationComponentLerpModeBase::GetInterpolatorModifierCache().lock();
	if (!l_interpolatorModifierCache) { return; }

	// マウスロックするかどうかを切り替える
	m_componentRotationByMouseHelper->ToggleMouseCenterLock();

	// もしマウスロックがされていなければ"return"
	if (m_componentRotationByMouseHelper->GetIsDisableMouseCenterLock()) { return; }

	const auto& l_application  = Application::GetInstance      ();
	const auto& l_inputManager = MRI::InputManager::GetInstance();

	// マウスの移動量を取得
	const Math::Vector3& l_mouseMovement = l_inputManager.FetchMouseDeltaAndResetCursorCenter();
	
	// マウスの移動量がほとんどないなら"return"
	if (l_mouseMovement.LengthSquared() >= CommonConstant::k_epsilon)
	{
		// "X"と"Y"の値を入れ替える("Y"方向の回転は"X","X"方向の回転は"Y"として扱うから)
		Math::Vector3 l_movement = { l_mouseMovement.y , l_mouseMovement.x , l_mouseMovement.z };

		// 回転速度にデルタタイムを乗算
		l_movement *= l_interpolatorModifierCache->GetCurrentValue() * l_application.GetScaledDeltaTime();

		// 現在のオイラー角を取得
		Math::Vector3 l_rotation = MRI::ComponentMode::RotationComponentModeBase::GetRotationDirection();

		// 回転適用が許されている軸にのみ回転を加算
		MRI::AxisUtility::AddAdaptDirection  (MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationAxisTagSet() , l_rotation , l_movement);
		MRI::AxisUtility::ResetAdaptDirection(MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationAxisTagSet() , l_rotation);

		// "X"軸の回転にのみ制限を掛ける(必要なら"3"軸全てにかけれるようにする)
		l_rotation.x = std::clamp(l_rotation.x , m_componentRotationByMouseHelper->GetMinRotatableDegreeX() , m_componentRotationByMouseHelper->GetMaxRotatableDegreeX());

		// 回転方向を格納
		MRI::ComponentMode::RotationComponentModeBase::SetRotationDirection(l_rotation);
	}

	
	// 現在の回転方向と目標の回転方向を取得
	Math::Quaternion l_currentQuaternion = l_selfTransformComponentCache->GetRotation();
	Math::Quaternion l_targetQuaternion = MRI::MathUtility::EulerToQuaternion       (MRI::ComponentMode::RotationComponentModeBase::GetRotationDirection());

	// 内積を取得
	// クオータニオンの最短経路補正
	if (l_currentQuaternion.Dot(l_targetQuaternion) <= CommonConstant::k_epsilon)
	{
		l_targetQuaternion = -l_targetQuaternion;
	}

	// 補完処理
	Math::Quaternion l_resultQuaternion = Math::Quaternion::Slerp(l_currentQuaternion , l_targetQuaternion , l_interpolatorModifierCache->GetCurrentValue() * l_application.GetScaledDeltaTime());

	// 回転方向を格納
	l_selfTransformComponentCache->SetRotation(l_resultQuaternion);

	l_interpolatorModifierCache->Update();
}

void MRI::ComponentMode::RotationComponentLerpMouseMode::EditPrefabInspector()
{
	MRI::ComponentMode::RotationComponentLerpModeBase::EditPrefabInspector();

	if (!m_componentRotationByMouseHelper) { return; }

	m_componentRotationByMouseHelper->EditPrefabInspector();
}

void MRI::ComponentMode::RotationComponentLerpMouseMode::DeserializePrefab(const nlohmann::json& a_json)
{
	MRI::ComponentMode::RotationComponentLerpModeBase::DeserializePrefab(a_json);

	if (!m_componentRotationByMouseHelper) { return; }

	m_componentRotationByMouseHelper->DeserializePrefab(a_json);
}

nlohmann::json MRI::ComponentMode::RotationComponentLerpMouseMode::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	if (!m_componentRotationByMouseHelper)
	{
		return nlohmann::json();
	}

	MRI::JsonUtility::UpdateJson(l_rootJson, m_componentRotationByMouseHelper->SerializePrefab                 ());
	MRI::JsonUtility::UpdateJson(l_rootJson, MRI::ComponentMode::RotationComponentLerpModeBase::SerializePrefab());

	return l_rootJson;
}