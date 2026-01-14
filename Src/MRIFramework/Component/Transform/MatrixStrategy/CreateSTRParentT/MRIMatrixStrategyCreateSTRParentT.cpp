#include "MRIMatrixStrategyCreateSTRParentT.h"

const MRI::TypeInfo& MRI::Strategy::MatrixStrategyCreateSTRParentT::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Strategy::MatrixStrategyCreateSTRParentT>();
}

void MRI::Strategy::MatrixStrategyCreateSTRParentT::Execute(std::weak_ptr<MRI::Component::TransformComponent>& a_ownerCache)
{
	auto l_ownerCache = a_ownerCache.lock();
	if (!l_ownerCache) { return; }

	auto l_parentCache = l_ownerCache->GetParentTransformComponentCache().lock();
	if (!l_parentCache) { return; }

	// 親の"Translation"を基準に下行列を作成(親に追従するような挙動)
	const Math::Matrix& l_resultMatrix = l_ownerCache->CalcScaleMatrix() * l_ownerCache->CalcTranslationMatrix() * l_ownerCache->CalcRotationMatrix() * l_parentCache->CalcTranslationMatrix();
	l_ownerCache->SetMatrix(l_resultMatrix);
}