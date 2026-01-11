#include "MRIMatrixStrategyCreateSRT.h"

const MRI::TypeInfo& MRI::Strategy::MatrixStrategyCreateSRT::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Strategy::MatrixStrategyCreateSRT>();
}

void MRI::Strategy::MatrixStrategyCreateSRT::Execute(std::weak_ptr<MRI::Component::TransformComponent>& a_owner)
{
	auto l_owner = a_owner.lock();
	if (!l_owner) { return; }

	Math::Matrix l_result = l_owner->CalcScaleMatrix() * l_owner->CalcRotationMatrix() * l_owner->CalcTranslationMatrix();
	l_owner->SetMatrix(l_result);
}