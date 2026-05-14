#include "MRIMoveComponentLerpFacingDirectionMode.h"

#include "../../../../../Application/main.h"

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

	// 入力方向を取得
	const auto& l_inputMoveDirection = MRI::ComponentMode::MoveComponentModeBase::GetMoveDirection();

	// 入力が無いなら補完の進捗をリセットして"return"
	if (l_inputMoveDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon)
	{
		l_interpolatorModifierCache->ResetInterpolate();
		return;
	}

	// 自分自身の回転行列を取得
	Math::Matrix l_rotationMatrix = l_selfTransformComponentCache->CalcRotationMatrix();

	// ローカル空間の正面方向を、ワールド空間の正面方向に変換
	Math::Vector3 l_resultMoveDirection = Math::Vector3::TransformNormal(Math::Vector3::Forward, l_rotationMatrix);

	// 使用しない軸の値を"0.0F"にして使用しないようにする
	MRI::AxisUtility::ResetUnusedAxis(MRI::ComponentMode::MoveComponentModeBase::GetWorkAdaptMoveAxisTagSet(), l_resultMoveDirection);

	// 進行方向が小さければ、補完の進捗をリセットして"return"
	if (l_resultMoveDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon)
	{
		l_interpolatorModifierCache->ResetInterpolate();
		return;
	}

	// 進行方向を正規化
	l_resultMoveDirection.Normalize();

	// 最終的な移動量を計算
	Math::Vector3 l_resultMovement = l_resultMoveDirection * l_interpolatorModifierCache->GetCurrentValue() * Application::GetInstance().GetScaledDeltaTime();
	const auto& l_resultPos = l_selfTransformComponentCache->GetPos() + l_resultMovement;

	// 現在の座標と移動量を足しこんだ座標を格納
	l_selfTransformComponentCache->SetPos(l_resultPos);

	l_interpolatorModifierCache->Update();
}