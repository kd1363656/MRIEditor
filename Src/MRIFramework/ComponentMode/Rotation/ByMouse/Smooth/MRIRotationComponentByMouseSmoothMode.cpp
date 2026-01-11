#include "MRIRotationComponentByMouseSmoothMode.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentByMouseSmoothMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentByMouseSmoothMode>();
}

void MRI::ComponentMode::RotationComponentByMouseSmoothMode::Update()
{
	auto l_selfTransformComponentCache = MRI::ComponentMode::RotationComponentByMouseModeBase::GetWorkSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	MRI::ComponentMode::RotationComponentByMouseModeBase::Update();

	// マウスロック中なら"return"
	if (MRI::ComponentMode::RotationComponentByMouseModeBase::GetDisableMouseLock()) { return; }

	// 現在の回転方向と目標の回転方向を取得
	const Math::Quaternion& l_currentRotation = l_selfTransformComponentCache->GetRotation();
	Math::Quaternion		l_targetRotation  = MRI::MathUtility::EulerToQuaternion       (MRI::ComponentMode::RotationComponentByMouseModeBase::GetTargetRotation());
	
	// クオータニオン補正(最短経路を決める)
	if (l_currentRotation.Dot(l_targetRotation) <= MRI::CommonConstant::k_epsilon)
	{
		l_targetRotation = -l_targetRotation;
	}

	// カメラの回転方向を格納
	Math::Quaternion l_resultRotation = Math::Quaternion::Slerp(l_currentRotation , l_targetRotation , MRI::ComponentMode::RotationComponentByMouseModeBase::GetRotationSpeed());
	l_selfTransformComponentCache->SetRotation				   (l_resultRotation);
}