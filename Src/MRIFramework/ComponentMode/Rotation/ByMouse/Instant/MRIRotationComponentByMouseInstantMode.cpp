#include "MRIRotationComponentByMouseInstantMode.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentByMouseInstantMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentByMouseInstantMode>();
}

void MRI::ComponentMode::RotationComponentByMouseInstantMode::Update()
{
	auto l_selfTransformComponentCache = MRI::ComponentMode::RotationComponentByMouseModeBase::GetWorkSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	MRI::ComponentMode::RotationComponentByMouseModeBase::Update();

	// マウスロック中なら"return"
	if (MRI::ComponentMode::RotationComponentByMouseModeBase::GetDisableMouseLock()) { return; }

	// オイラー角をクオータニオンに変換
	const auto& l_resultRotation = MRI::MathUtility::EulerToQuaternion(MRI::ComponentMode::RotationComponentByMouseModeBase::GetTargetRotation());
	l_selfTransformComponentCache->SetRotation						  (l_resultRotation);
}