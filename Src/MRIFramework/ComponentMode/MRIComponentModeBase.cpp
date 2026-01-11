#include "MRIComponentModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::ComponentModeBase::GetTypeInfo() const 
{
	return MRI::GetTypeInfo<MRI::ComponentMode::ComponentModeBase>();
}