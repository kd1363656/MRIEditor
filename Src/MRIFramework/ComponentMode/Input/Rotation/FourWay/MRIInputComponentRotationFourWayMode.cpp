#include "MRIInputComponentRotationFourWayMode.h"

const MRI::TypeInfo& MRI::ComponentMode::InputComponentRotationFourWayMode::GetTypeInfo() const
{	
	return MRI::GetTypeInfo<MRI::ComponentMode::InputComponentRotationFourWayMode>();
}

void MRI::ComponentMode::InputComponentRotationFourWayMode::EarlyUpdate()
{
	auto l_selfRotationComponentCache = MRI::ComponentMode::InputComponentRotationModeBase::GetWorkSelfRotationComponentCache().lock();
	if (!l_selfRotationComponentCache) { return; }

	auto l_rotationComponentModeCache = l_selfRotationComponentCache->GetRotationComponentModeCache().lock();
	if (!l_rotationComponentModeCache) { return; }

	MRI::AxisUtility::ApplyDirectionIfKeyHeld<MRI::Tag::InputRightEventTag>   (l_rotationComponentModeCache->GetWorkRotationDirection() , Math::Vector3::Right);
	MRI::AxisUtility::ApplyDirectionIfKeyHeld<MRI::Tag::InputLeftEventTag>    (l_rotationComponentModeCache->GetWorkRotationDirection() , Math::Vector3::Left);
	MRI::AxisUtility::ApplyDirectionIfKeyHeld<MRI::Tag::InputBackwardEventTag>(l_rotationComponentModeCache->GetWorkRotationDirection() , Math::Vector3::Backward);
	MRI::AxisUtility::ApplyDirectionIfKeyHeld<MRI::Tag::InputForwardEventTag> (l_rotationComponentModeCache->GetWorkRotationDirection() , Math::Vector3::Forward);
}