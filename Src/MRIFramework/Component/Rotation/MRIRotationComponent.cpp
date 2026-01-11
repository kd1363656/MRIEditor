#include "MRIRotationComponent.h"

const MRI::TypeInfo& MRI::Component::RotationComponent::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::RotationComponent>();
}