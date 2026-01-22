#include "MRIRotationComponentLerpTargetViewDirectionMode.h"

#include "../../../../../Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode>();
}

void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::Init()
{
	MRI::ComponentMode::RotationComponentLerpModeBase::Init();

	if (!m_targetTransformComponentFetcher) 
	{
		m_targetTransformComponentFetcher = std::make_shared<MRI::Helper::ComponentUUIDFetcherHelper<MRI::Component::TransformComponent>>();
	}

	m_targetTransformComponentFetcher->Init();
}

void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner)
{
	if (!a_owner)                           { return; }
	if (!m_targetTransformComponentFetcher) { return; }
	
	MRI::ComponentMode::RotationComponentLerpModeBase::PostLoadInit(a_owner);

	m_targetTransformComponentFetcher->PostLoadInit(a_owner);
}

void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::PreUpdate()
{
	// 移動方向の入力をクリア
	MRI::ComponentMode::RotationComponentModeBase::ResetRotationDirection();
}
void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::Update()
{
	if (!m_targetTransformComponentFetcher) { return; }

	auto l_targetTransformComponentCache = m_targetTransformComponentFetcher->Get().lock();
	if (!l_targetTransformComponentCache) { return; }

	auto l_selfTransformComponentCache = MRI::ComponentMode::RotationComponentModeBase::GetWorkSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	auto l_interpolatorModifierCache = MRI::ComponentMode::RotationComponentLerpModeBase::GetInterpolatorModifierCache().lock();
	if (!l_interpolatorModifierCache) { return; }

	// もし入力方向がなければ補完進捗度を初期化して"return"
	Math::Vector3 l_rotationDirection = MRI::ComponentMode::RotationComponentLerpModeBase::GetRotationDirection();
	if (l_rotationDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon)
	{
		l_interpolatorModifierCache->ResetInterpolate();
		return;
	}

	// 使用しない回転ベクトルを初期化
	MRI::AxisUtility::ResetUnusedAxis(MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationAxisTagSet() , l_rotationDirection);

	if (l_rotationDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon) { return; }
	
	// 回転ベクトルを正規化
	l_rotationDirection.Normalize();

	// 基準となるゲームオブジェクトの回転を回転行列から取得
	Math::Vector3 l_targetForward = l_targetTransformComponentCache->CalcRotationMatrix().Forward();

	// このゲームオブジェクトが使用しない軸は使用しない
	MRI::AxisUtility::ResetUnusedAxis(MRI::ComponentMode::RotationComponentModeBase::GetWorkAdaptRotationAxisTagSet() , l_targetForward);

	// 回転対象の"Yaw"角を取得
	float l_targetYaw = MRI::MathUtility::VectorToYaw(l_targetForward);

	// 入力方向をターゲット基準で回転
	Math::Matrix  l_yawMatrix      = Math::Matrix::CreateRotationY (l_targetYaw);
	Math::Vector3 l_worldDirection = Math::Vector3::TransformNormal(l_rotationDirection , l_yawMatrix);

	// "XZ"平面に制限し正規化
	l_worldDirection.y = 0.0F;
	if (l_worldDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon) { return; }
	l_worldDirection.Normalize();

	// ワールド方向から"Yaw"角を取得
	const float l_worldTargetYaw = MRI::MathUtility::VectorToYaw(l_worldDirection);

	// "Yaw"角からクオータニオンを生成
	Math::Quaternion l_targetQuaternion = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::Up , l_worldTargetYaw);

	// 現在の回転
	Math::Quaternion l_currentQuaternion = l_selfTransformComponentCache->GetRotation();

	// クオータニオンの内積が"0.0f"以下なら最短経路じゃなくなってしまうため符号を反転する
	if (l_currentQuaternion.Dot(l_targetQuaternion) < 0.0f)
	{
		l_targetQuaternion = -l_targetQuaternion;
	}


	float l_deltaTime = Application::GetInstance().GetScaledDeltaTime();

	// 球面線形補正
	Math::Quaternion l_resultQuaternion = Math::Quaternion::Slerp(l_currentQuaternion , l_targetQuaternion , l_interpolatorModifierCache->GetCurrentValue() * l_deltaTime);

	l_selfTransformComponentCache->SetRotation(l_resultQuaternion);

	l_interpolatorModifierCache->Update();
}

void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::EditSpawnInspector()
{
	if (!m_targetTransformComponentFetcher) { return; }
	m_targetTransformComponentFetcher->EditPrefabInspector();
}

void MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::DeserializeSpawn(const nlohmann::json& a_json)
{
	if (a_json.is_null())                   { return; }
	if (!m_targetTransformComponentFetcher) { return; }

	m_targetTransformComponentFetcher->DeserializeSpawn(a_json);
}

nlohmann::json MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode::SerializeSpawn()
{
	auto l_rootJson = nlohmann::json();

	if (!m_targetTransformComponentFetcher)
	{
		return nlohmann::json();
	}

	MRI::JsonUtility::UpdateJson(l_rootJson , m_targetTransformComponentFetcher->SerializeSpawn());

	return l_rootJson;
}