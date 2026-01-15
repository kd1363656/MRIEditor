#include "MRIMoveComponentConstantSpeedFacingDirectionMode.h"

#include "../../../../../Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::MoveComponentConstantSpeedFacingDirectionMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentConstantSpeedFacingDirectionMode>();
}

void MRI::ComponentMode::MoveComponentConstantSpeedFacingDirectionMode::PreUpdate()
{
	// 移動方向をリセット
	MRI::ComponentMode::MoveComponentModeBase::ResetMoveDirection();
}

void MRI::ComponentMode::MoveComponentConstantSpeedFacingDirectionMode::Update()
{
	auto l_selfTransformComponentCache = MRI::ComponentMode::MoveComponentModeBase::GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	// 進行方向を取得し正規化
	Math::Vector3 l_resultMoveDirection = MRI::ComponentMode::MoveComponentModeBase::CalcMoveDirection();

	// 進行方向が変わらないなら"return"
	if (l_resultMoveDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon)
	{
		return;
	}

	l_resultMoveDirection.Normalize();

	// 最終的な移動量を計算
	Math::Vector3 l_resultMovement = l_resultMoveDirection                   * MRI::ComponentMode::MoveComponentConstantSpeedModeBase::GetMoveSpeed() * Application::GetInstance().GetScaledDeltaTime();
	const auto&   l_resultPos      = l_selfTransformComponentCache->GetPos() + l_resultMovement;

	// 現在の座標と移動量を足しこんだ座標を格納
	l_selfTransformComponentCache->SetPos(l_resultPos);
}