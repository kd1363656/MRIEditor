#include "MRIMoveComponentLerpTargetViewDirectionMode.h"

#include "../../../../../Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::MoveComponentLerpTargetViewDirectionMode::GetTypeInfo() const
{	
	return MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentLerpTargetViewDirectionMode>();
}

void MRI::ComponentMode::MoveComponentLerpTargetViewDirectionMode::Init()
{
	MRI::ComponentMode::MoveComponentLerpModeBase::Init();

	if (!m_targetTransformComponentFetcher) 
	{
		m_targetTransformComponentFetcher = std::make_shared<MRI::Helper::ComponentUUIDFetcherHelper<MRI::Component::TransformComponent>>();
	}

	m_targetTransformComponentFetcher->Init();
}
void MRI::ComponentMode::MoveComponentLerpTargetViewDirectionMode::PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner)
{
	if (!a_owner)							{ return; }
	if (!m_targetTransformComponentFetcher) { return; }

	MRI::ComponentMode::MoveComponentLerpModeBase::PostLoadInit(a_owner);

	m_targetTransformComponentFetcher->PostLoadInit(a_owner);
}

void MRI::ComponentMode::MoveComponentLerpTargetViewDirectionMode::PreUpdate()
{
	MRI::ComponentMode::MoveComponentModeBase::ResetMoveDirection();
}

void MRI::ComponentMode::MoveComponentLerpTargetViewDirectionMode::Update()
{
	if (!m_targetTransformComponentFetcher) { return; }

	auto l_selfTransformComponentCache = MRI::ComponentMode::MoveComponentModeBase::GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	auto l_interpolatorModifierCache = MRI::ComponentMode::MoveComponentLerpModeBase::GetInterpolatorModifierCache().lock();
	if (!l_interpolatorModifierCache) { return; }

	const Math::Vector3& l_resultMoveDirection = CalcMoveDirection();

	if (l_resultMoveDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon) 
	{
		// 補完の更新をリセット
		l_interpolatorModifierCache->ResetInterpolate();
		return; 
	}

	const float   l_deltaTime  = Application::GetInstance().GetScaledDeltaTime();
	Math::Vector3 l_resultMove = l_resultMoveDirection * l_interpolatorModifierCache->GetCurrentValue() * l_deltaTime;

	// 自身の座標に移動量を加算
	const Math::Vector3& l_resultPos = l_selfTransformComponentCache->GetPos() + l_resultMove;
	l_selfTransformComponentCache->SetPos(l_resultPos);

	// 補完の更新
	l_interpolatorModifierCache->Update();
}

void MRI::ComponentMode::MoveComponentLerpTargetViewDirectionMode::EditSpawnInspector()
{
	if (!m_targetTransformComponentFetcher) { return; }
	m_targetTransformComponentFetcher->EditPrefabInspector();
}

void MRI::ComponentMode::MoveComponentLerpTargetViewDirectionMode::DeserializeSpawn(const nlohmann::json& a_json)
{
	if (a_json.is_null())                   { return; }
	if (!m_targetTransformComponentFetcher) { return; }

	m_targetTransformComponentFetcher->DeserializeSpawn(a_json);
}

nlohmann::json MRI::ComponentMode::MoveComponentLerpTargetViewDirectionMode::SerializeSpawn()
{
	auto l_rootJson = nlohmann::json();

	if (!m_targetTransformComponentFetcher)
	{
		return nlohmann::json();
	}

	MRI::JsonUtility::UpdateJson(l_rootJson, m_targetTransformComponentFetcher->SerializeSpawn());

	return l_rootJson;
}

Math::Vector3 MRI::ComponentMode::MoveComponentLerpTargetViewDirectionMode::CalcMoveDirection()
{
	Math::Vector3 l_moveDirection = MRI::ComponentMode::MoveComponentModeBase::GetMoveDirection();
	if (l_moveDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon)
	{
		return Math::Vector3::Zero;
	}

	if (!m_targetTransformComponentFetcher) 
	{
		return Math::Vector3::Zero; 
	}

	auto l_targetTransformComponentCache = m_targetTransformComponentFetcher->Get().lock();
	if (!l_targetTransformComponentCache) 
	{
		return Math::Vector3::Zero;  
	}

	Math::Matrix l_targetRotationMatrix = l_targetTransformComponentCache->CalcRotationMatrix();
	
	// 移動方向の正規化
	l_moveDirection.Normalize();	

	// 行列を使って移動ベクトルを回転
	Math::Vector3 l_rotationMoveDirection = Math::Vector3::TransformNormal(l_moveDirection , l_targetRotationMatrix);
	
	// 使わない軸は無効化
	MRI::AxisUtility::ResetUnusedAxis(MRI::ComponentMode::MoveComponentModeBase::GetWorkAdaptMoveAxisTagSet() , l_rotationMoveDirection);
	l_rotationMoveDirection.Normalize();

	return l_rotationMoveDirection;
}