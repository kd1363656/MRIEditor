#include "MRIInputComponentModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::InputComponentModeBase::GetTypeInfo() const
{	
	return MRI::GetTypeInfo<MRI::ComponentMode::InputComponentModeBase>();
}