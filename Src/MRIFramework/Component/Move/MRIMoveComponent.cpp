#include "MRIMoveComponent.h"

const MRI::TypeInfo& MRI::Component::MoveComponent::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::MoveComponent>();
}