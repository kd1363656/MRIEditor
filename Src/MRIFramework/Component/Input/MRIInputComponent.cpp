#include "MRIInputComponent.h"

#include "Application/main.h"

const MRI::TypeInfo& MRI::Component::InputComponent::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::InputComponent>();
}

void MRI::Component::InputComponent::Init()
{

}
void MRI::Component::InputComponent::PostLoadInit()
{
	auto l_ownerCache = MRI::Component::ComponentBase::GetWorkOwnerCache().lock();
	if (!l_ownerCache) { return; }


}

void MRI::Component::InputComponent::EarlyUpdate()
{	
	const auto& l_playerInputConfigCache = SceneManager::GetInstance().GetPlayerKeyConfigCache().lock();
	if (!l_playerInputConfigCache) { return; }

}