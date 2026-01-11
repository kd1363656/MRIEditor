#include "MRIMatrixStrategyBase.h"

const MRI::TypeInfo& MRI::Strategy::MatrixStrategyBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Strategy::MatrixStrategyBase>();
}