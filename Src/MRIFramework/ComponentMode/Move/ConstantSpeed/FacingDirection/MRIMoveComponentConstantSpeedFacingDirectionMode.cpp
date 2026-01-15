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

	auto l_moveDirection = MRI::ComponentMode::MoveComponentModeBase::GetMoveDirection();

	// 移動方向が格納されていないか値が小さければ"return"
	if (l_moveDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon) { return; }

	const float l_moveSpeed = MRI::ComponentMode::MoveComponentConstantSpeedModeBase::GetMoveSpeed() * Application::GetInstance().GetScaledDeltaTime();

	// 移動方向を正規化
	l_moveDirection.Normalize();

	// このゲームオブジェクトの正面方向から見て向くべき方向を計算
	Math::Matrix  l_rotationMatrix      = l_selfTransformComponentCache->CalcRotationMatrix();
	Math::Vector3 l_resultMoveDirection = Math::Vector3::TransformNormal                   (l_moveDirection , l_rotationMatrix);

	// 使用しない軸の値を"0.0F"にして使用しないようにする
	MRI::AxisUtility::ResetUnusedAxis(MRI::ComponentMode::MoveComponentConstantSpeedModeBase::GetWorkAdaptMoveAxisTagSet() , l_resultMoveDirection);

	// 使用しない軸の移動方向を"0.0F"にしてから正規化
	l_resultMoveDirection.Normalize();

	// 最終的な移動量を計算
	Math::Vector3 l_resultMovement = l_resultMoveDirection * l_moveSpeed;
	const auto&   l_resultPos      = l_selfTransformComponentCache->GetPos() + l_resultMovement;

	// 現在の座標と移動量を足しこんだ座標を格納
	l_selfTransformComponentCache->SetPos(l_resultPos);
}