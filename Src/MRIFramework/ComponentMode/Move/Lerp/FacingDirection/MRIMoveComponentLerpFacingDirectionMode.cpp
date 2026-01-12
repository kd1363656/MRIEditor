#include "MRIMoveComponentLerpFacingDirectionMode.h"

#include "Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::MoveComponentLerpFacingDirectionMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentLerpFacingDirectionMode>();
}

void MRI::ComponentMode::MoveComponentLerpFacingDirectionMode::Update()
{
	const auto& l_inputManager = MRI::InputManager::GetInstance();

	auto l_selfTransformComponentCache = MRI::ComponentMode::MoveComponentModeBase::GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	auto l_interpolatorModifierCache = MRI::ComponentMode::MoveComponentLerpModeBase::GetInterpolatorModifier().lock();
	if (!l_interpolatorModifierCache) { return; }

	auto l_moveDirection = MRI::ComponentMode::MoveComponentModeBase::GetMoveDirection();
	if (l_moveDirection.LengthSquared() <= CommonConstant::k_epsilon) 
	{
		// 入力がなければ補完速度をリセット
		l_interpolatorModifierCache->ResetInterpolate();
		return; 
	}

	const float l_deltaTime = Application::GetInstance().GetScaledDeltaTime();

	// 移動方向を正規化
	l_moveDirection.Normalize();

	// 向いてる方向から見て右、左、前、後ろ、どちらに移動するかを取得
	Math::Matrix  l_rotationMat         = l_selfTransformComponentCache->CalcRotationMatrix();
	Math::Vector3 l_resultMoveDirection = Math::Vector3::TransformNormal                    (l_moveDirection , l_rotationMat);

	// "Y"座標は移動しない
	l_resultMoveDirection.y = 0.0F;

	// 確定した移動方向をノーマライズ
	l_resultMoveDirection.Normalize();

	// 最終的な移動量を計算して現在の座標と足しこんだ座標を格納
	Math::Vector3 l_resultMovement = l_resultMoveDirection					 * l_interpolatorModifierCache->GetCurrentValue() * l_deltaTime;
	const auto&   l_resultPos      = l_selfTransformComponentCache->GetPos() + l_resultMovement;
	l_selfTransformComponentCache->SetPos								  (l_resultPos);

	// 移動量を更新
	l_interpolatorModifierCache->Update();
}