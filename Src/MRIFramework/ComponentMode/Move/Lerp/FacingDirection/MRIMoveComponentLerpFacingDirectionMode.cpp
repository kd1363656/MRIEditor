#include "MRIMoveComponentLerpFacingDirectionMode.h"

const MRI::TypeInfo& MRI::ComponentMode::MoveComponentLerpFacingDirectionMode::GetTypeInfo() const
{	
	return MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentLerpFacingDirectionMode>();
}

void MRI::ComponentMode::MoveComponentLerpFacingDirectionMode::PreUpdate()
{
	MRI::ComponentMode::MoveComponentModeBase::ResetMoveDirection();
}

void MRI::ComponentMode::MoveComponentLerpFacingDirectionMode::Update()
{
	auto l_selfTransformComponentCache = MRI::ComponentMode::MoveComponentModeBase::GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	auto l_interpolatorModifierCache = MRI::ComponentMode::MoveComponentLerpModeBase::GetInterpolatorModifierCache().lock();
	if (!l_interpolatorModifierCache) { return; }

	// 進行方向を取得し正規化
	Math::Vector3 l_resultMoveDirection = MRI::ComponentMode::MoveComponentModeBase::CalcMoveDirection();

	// 進行方向が変わらないなら補完の進捗をリセットして"return"
	if (l_resultMoveDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon)
	{
		l_interpolatorModifierCache->ResetInterpolate();
		return;
	}

	l_resultMoveDirection.Normalize();

	// 最終的な移動量を計算
	Math::Vector3 l_resultMovement = l_resultMoveDirection                   * l_interpolatorModifierCache->GetCurrentValue();
	const auto&   l_resultPos      = l_selfTransformComponentCache->GetPos() + l_resultMovement;

	// 現在の座標と移動量を足しこんだ座標を格納
	l_selfTransformComponentCache->SetPos(l_resultPos);

	l_interpolatorModifierCache->Update();
}