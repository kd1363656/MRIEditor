#include "MRIRotationComponentInstantByMouseMode.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentInstantByMouseMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentInstantByMouseMode>();
}

void MRI::ComponentMode::RotationComponentInstantByMouseMode::Update()
{
	auto l_selfTransformComponentCache = MRI::ComponentMode::RotationComponentModeBase::GetWorkSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	MRI::ComponentMode::RotationComponentByMouseModeBase::Update();

	// マウスロックが解除されていれば実行
	if (MRI::ComponentMode::RotationComponentByMouseModeBase::GetIsDisableMouseCenterLock()) { return; }

	// オイラー角をクオータニオンに変換
	const auto& l_resultRotation = MRI::MathUtility::EulerToQuaternion(MRI::ComponentMode::RotationComponentModeBase::GetRotationDirection());
	l_selfTransformComponentCache->SetRotation                        (l_resultRotation);
}