#include "MRIFactoryUtility.h"

void MRI::FactoryUtility::RegisterAll()
{
	RegisterStrategyFactory            ();
	RegisterComponentFactory           ();
	RegisterInterpolatorModifierFactory();
	RegisterObserverNotifierFactory    ();
	RegisterComponentModeFactory       ();
}

void MRI::FactoryUtility::RegisterComponentFactory()
{
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::Component , MRI::Component::CameraComponent);
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::Component , MRI::Component::RenderModelAnimationComponent);
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::Component , MRI::Component::RenderModelStaticComponent);
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::Component , MRI::Component::RotationComponent);
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::Component , MRI::Component::MoveComponent);
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::Component , MRI::Component::InputComponent);
}
void MRI::FactoryUtility::RegisterStrategyFactory()
{
	MRI_REGISTER_FACTORY_METHOD(MRI::UniqueFactory::Strategy<MRI::Strategy::MatrixStrategyBase> , MRI::Strategy::MatrixStrategyCreateSRT);
}
void MRI::FactoryUtility::RegisterInterpolatorModifierFactory()
{
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::InterpolatorModifier , MRI::Modifier::HermiteInterpolatorModifier);
}
void MRI::FactoryUtility::RegisterObserverNotifierFactory()
{
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::ObserverNotifier , MRI::Observer::ObserverNotifierKeep);
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::ObserverNotifier , MRI::Observer::ObserverNotifierMoment);
}
void MRI::FactoryUtility::RegisterComponentModeFactory()
{
	MRI_REGISTER_FACTORY_METHOD(MRI::UniqueFactory::MoveComponentMode     , MRI::ComponentMode::MoveComponentLinearFacingDirectionMode);
	MRI_REGISTER_FACTORY_METHOD(MRI::UniqueFactory::RotationComponentMode , MRI::ComponentMode::RotationComponentLinearByMouseMode);
}