#include "MRIMoveComponentModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::MoveComponentModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentModeBase>();
}

void MRI::ComponentMode::MoveComponentModeBase::Init()
{
	m_selfTransformComponentCache.reset();
}
void MRI::ComponentMode::MoveComponentModeBase::PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner)
{
	if (!a_owner) { return; }

	m_selfTransformComponentCache = a_owner->GetSelfTransformComponentCache();
}

void MRI::ComponentMode::MoveComponentModeBase::PreUpdate()
{
	m_moveDirection = Math::Vector3::Zero;
}