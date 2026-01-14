#include "MRIMatrixStrategyCreateSRT.h"

const MRI::TypeInfo& MRI::Strategy::MatrixStrategyCreateSRT::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Strategy::MatrixStrategyCreateSRT>();
}

void MRI::Strategy::MatrixStrategyCreateSRT::Execute(std::weak_ptr<MRI::Component::TransformComponent>& a_ownerCache)
{
	auto l_ownerCache = a_ownerCache.lock();
	if (!l_ownerCache) { return; }

	Math::Matrix l_result = l_ownerCache->CalcScaleMatrix() * l_ownerCache->CalcRotationMatrix() * l_ownerCache->CalcTranslationMatrix();
	l_ownerCache->SetMatrix(l_result);
}