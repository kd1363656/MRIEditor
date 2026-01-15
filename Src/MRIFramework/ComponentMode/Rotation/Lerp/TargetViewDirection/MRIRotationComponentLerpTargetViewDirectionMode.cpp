#include "MRIRotationComponentLerpTargetViewDirectionMode.h"

#include "../../../../../Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode>();
}

void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::Init()
{
	MRI::ComponentMode::RotationComponentLerpModeBase::Init();

	if (!m_targetTransformComponent) 
	{
		m_targetTransformComponent = std::make_shared<MRI::Helper::ComponentUUIDFetcherHelper<MRI::Component::TransformComponent>>();
	}

	m_targetTransformComponent->Init();
}

void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner)
{
	if (!a_owner)                    { return; }
	if (!m_targetTransformComponent) { return; }
	
	MRI::ComponentMode::RotationComponentLerpModeBase::PostLoadInit(a_owner);

	m_targetTransformComponent->PostLoadInit(a_owner);
}

void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::PreUpdate()
{
	// 移動方向の入力をクリア
	MRI::ComponentMode::RotationComponentModeBase::ResetRotationDirection();
}
void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::Update()
{
	if (MRI::InputManager::GetInstance().IsInputHeld('A'))
	{
		MRI::MathUtility::AddDirection(MRI::ComponentMode::RotationComponentModeBase::GetWorkRotationDirection() , Math::Vector3::Left);
	}
	else if (MRI::InputManager::GetInstance().IsInputHeld('D'))
	{
		MRI::MathUtility::AddDirection(MRI::ComponentMode::RotationComponentModeBase::GetWorkRotationDirection() , Math::Vector3::Right);
	}

	if (MRI::InputManager::GetInstance().IsInputHeld('W'))
	{
		MRI::MathUtility::AddDirection(MRI::ComponentMode::RotationComponentModeBase::GetWorkRotationDirection(), Math::Vector3::Backward);
	}
	else if (MRI::InputManager::GetInstance().IsInputHeld('S'))
	{
		MRI::MathUtility::AddDirection(MRI::ComponentMode::RotationComponentModeBase::GetWorkRotationDirection(), Math::Vector3::Forward);
	}

	if (!m_targetTransformComponent) { return; }

	auto l_interpolatorModifierCache = MRI::ComponentMode::RotationComponentLerpModeBase::GetInterpolatorModifierCache().lock();
	if (!l_interpolatorModifierCache) { return; }

	// もし入力方向がなければ補完進捗度を初期化して"return"
	Math::Vector3 l_rotationDirection = MRI::ComponentMode::RotationComponentLerpModeBase::GetRotationDirection();
	if (l_rotationDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon)
	{
		l_interpolatorModifierCache->ResetInterpolate();
		return;
	}

	l_rotationDirection.Normalize();

	// 指針のコンポーネントと回転の基準とするゲームオブジェクトのコンポーネントのヌルチェック
	auto l_selfTransformComponentCache   = MRI::ComponentMode::RotationComponentModeBase::GetWorkSelfTransformComponentCache().lock();
	auto l_targetTransformComponentCache = m_targetTransformComponent->Get													().lock();

	if (!l_selfTransformComponentCache || !l_targetTransformComponentCache) { return; }

	// 基準となる"Yaw"角を取得
	Math::Vector3 l_targetForward = l_targetTransformComponentCache->CalcRotationMatrix().Forward();
	
	// 使わない軸の方向を無効化
	MRI::AxisUtility::ResetUnusedAxis(MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationAxisTagSet() , l_targetForward);
	
	if (l_targetForward.LengthSquared() <= MRI::CommonConstant::k_epsilon){ return; }

	l_targetForward.Normalize							   ();
	const float l_targetYaw = MRI::MathUtility::VectorToYaw(l_targetForward);

	// 回転方向に基準の"Yaw"で回転
	Math::Matrix  l_yawMatrix      = Math::Matrix::CreateRotationY (l_targetYaw);
	Math::Vector3 l_worldDirection = Math::Vector3::TransformNormal(l_rotationDirection , l_yawMatrix);;

	// 使わない軸の方向を無効化
	MRI::AxisUtility::ResetUnusedAxis(MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationAxisTagSet() , l_worldDirection);
	
	if (l_worldDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon) { return; }

	l_worldDirection.Normalize();

	// ワールド方向から"Yaw"を取得しクオータニオンを計算
	const float      l_newYaw           = MRI::MathUtility::VectorToYaw        (l_worldDirection);
	Math::Quaternion l_targetQuaternion = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::Up , l_newYaw);

	Math::Quaternion l_currentQuaternion = l_selfTransformComponentCache->GetRotation();
	if (l_currentQuaternion.Dot(l_targetQuaternion) < MRI::CommonConstant::k_epsilon)
	{
		l_targetQuaternion = -l_targetQuaternion;
	}

	const float l_deltaTime = Application::GetInstance().GetScaledDeltaTime();

	Math::Quaternion l_resultQuaternion = Math::Quaternion::Slerp(l_currentQuaternion , l_targetQuaternion , l_interpolatorModifierCache->GetCurrentValue() * l_deltaTime);

	l_selfTransformComponentCache->SetRotation(l_resultQuaternion);
	l_interpolatorModifierCache->Update       ();
}

void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::EditSpawnInspector()
{
	if (!m_targetTransformComponent) { return; }
	m_targetTransformComponent->EditPrefabInspector();
}

void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::DeserializeSpawn(const nlohmann::json& a_json)
{
	if (a_json.is_null())            { return; }
	if (!m_targetTransformComponent) { return; }

	m_targetTransformComponent->DeserializeSpawn(a_json);
}

nlohmann::json MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::SerializeSpawn()
{
	auto l_rootJson = nlohmann::json();

	if (!m_targetTransformComponent)
	{
		return nlohmann::json();
	}

	MRI::JsonUtility::UpdateJson(l_rootJson , m_targetTransformComponent->SerializeSpawn());

	return l_rootJson;
}