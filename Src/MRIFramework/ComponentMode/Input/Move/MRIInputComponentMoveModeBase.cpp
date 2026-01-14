#include "MRIInputComponentMoveModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::InputComponentMoveModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::InputComponentMoveModeBase>();
}

void MRI::ComponentMode::InputComponentMoveModeBase::PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner)
{
	if (!a_owner) { return; }

	m_selfMoveComponentCache = a_owner->GetComponentCache<MRI::Component::MoveComponent>();
}