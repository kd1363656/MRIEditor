#include "MRIInputComponentMoveFourWayMode.h"

const MRI::TypeInfo& MRI::ComponentMode::InputComponentMoveFourWayMode::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::InputComponentMoveFourWayMode>();
}

void MRI::ComponentMode::InputComponentMoveFourWayMode::EarlyUpdate()
{
	auto l_selfMoveComponentCache = MRI::ComponentMode::InputComponentMoveModeBase::GetWorkSelfMoveComponentCache().lock();
	if (!l_selfMoveComponentCache) { return; }

	auto l_moveComponentModeCache = l_selfMoveComponentCache->GetMoveComponentModeCache().lock();
	if (!l_moveComponentModeCache) { return; }

	MRI::AxisUtility::ApplyDirectionIfKeyHeld<MRI::Tag::InputRightEventTag>   (l_moveComponentModeCache->GetWorkMoveDirection() , Math::Vector3::Right);
	MRI::AxisUtility::ApplyDirectionIfKeyHeld<MRI::Tag::InputLeftEventTag>    (l_moveComponentModeCache->GetWorkMoveDirection() , Math::Vector3::Left);
	MRI::AxisUtility::ApplyDirectionIfKeyHeld<MRI::Tag::InputBackwardEventTag>(l_moveComponentModeCache->GetWorkMoveDirection() , Math::Vector3::Backward);
	MRI::AxisUtility::ApplyDirectionIfKeyHeld<MRI::Tag::InputForwardEventTag> (l_moveComponentModeCache->GetWorkMoveDirection() , Math::Vector3::Forward);
}