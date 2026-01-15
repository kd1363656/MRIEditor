#include "MRIInputComponentRotationModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::InputComponentRotationModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::InputComponentRotationModeBase>();
}

void MRI::ComponentMode::InputComponentRotationModeBase::PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner)
{
	if (!a_owner) { return; }

	m_selfRotationComponentCache = a_owner->GetComponentCache<MRI::Component::RotationComponent>();
}