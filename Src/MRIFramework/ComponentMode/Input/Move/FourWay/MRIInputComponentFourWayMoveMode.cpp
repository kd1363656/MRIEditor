#include "MRIInputComponentFourWayMoveMode.h"

const MRI::TypeInfo& MRI::ComponentMode::InputComponentFourWayMoveMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::InputComponentFourWayMoveMode>();
}

void MRI::ComponentMode::InputComponentFourWayMoveMode::EarlyUpdate()
{
	auto l_selfMoveComponentCache = MRI::ComponentMode::InputComponentMoveModeBase::GetWorkSelfMoveComponentCache().lock();
	if (!l_selfMoveComponentCache) { return; }

	auto l_moveComponentModeCache = l_selfMoveComponentCache->GetMoveComponentModeCache().lock();
	if (!l_moveComponentModeCache) { return; }

	AddMoveInputIfKeyPressed<MRI::Tag::InputRightEventTag>   (l_moveComponentModeCache->GetWorkMoveDirection() , Math::Vector3::Right);
	AddMoveInputIfKeyPressed<MRI::Tag::InputLeftEventTag>    (l_moveComponentModeCache->GetWorkMoveDirection() , Math::Vector3::Left);
	AddMoveInputIfKeyPressed<MRI::Tag::InputBackwardEventTag>(l_moveComponentModeCache->GetWorkMoveDirection() , Math::Vector3::Backward);
	AddMoveInputIfKeyPressed<MRI::Tag::InputForwardEventTag> (l_moveComponentModeCache->GetWorkMoveDirection() , Math::Vector3::Forward);
}