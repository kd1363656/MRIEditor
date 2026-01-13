#include "MRIRotationComponentSmoothByMouseMode.h"

#include "../../../../../Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentSmoothByMouseMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentSmoothByMouseMode>();
}

void MRI::ComponentMode::RotationComponentSmoothByMouseMode::Update()
{
	auto l_selfTransformComponentCache = MRI::ComponentMode::RotationComponentModeBase::GetWorkSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	MRI::ComponentMode::RotationComponentByMouseModeBase::Update();

	// マウスロックが解除されていれば実行
	if (MRI::ComponentMode::RotationComponentByMouseModeBase::GetIsDisableMouseCenterLock()) { return; }

	// 現在の回転と回転予定方向を取得
	Math::Quaternion l_currentQuaternion = l_selfTransformComponentCache->GetRotation();
	Math::Quaternion l_targetQuaternion  = MRI::MathUtility::EulerToQuaternion       (MRI::ComponentMode::RotationComponentModeBase::GetRotationDirection());

	// クオータニオン補正(最短経路を決める)
	if (l_currentQuaternion.Dot(l_targetQuaternion) <= CommonConstant::k_epsilon)
	{
		l_targetQuaternion = -l_targetQuaternion;
	}

	// 補完処理
	Math::Quaternion l_resultQuaternion = Math::Quaternion::Slerp(l_currentQuaternion , l_targetQuaternion , MRI::ComponentMode::RotationComponentByMouseModeBase::GetRotationSpeed() * Application::GetInstance().GetScaledDeltaTime());

	// オイラー角をクオータニオンに変換
	l_selfTransformComponentCache->SetRotation(l_resultQuaternion);
}