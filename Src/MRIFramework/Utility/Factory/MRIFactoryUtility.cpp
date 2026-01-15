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
	MRI_REGISTER_FACTORY_METHOD(MRI::UniqueFactory::Strategy<MRI::Strategy::MatrixStrategyBase> , MRI::Strategy::MatrixStrategyCreateSTRParentT);
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
	// 移動
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::MoveComponentMode , MRI::ComponentMode::MoveComponentConstantSpeedFacingDirectionMode);
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::MoveComponentMode , MRI::ComponentMode::MoveComponentLerpFacingDirectionMode);
	
	// 回転
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::RotationComponentMode , MRI::ComponentMode::RotationComponentConstantSpeedMouseMode);
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::RotationComponentMode , MRI::ComponentMode::RotationComponentLerpMouseMode);
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::RotationComponentMode , MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode);

	// 入力
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::InputComponentMode , MRI::ComponentMode::InputComponentMoveFourWayMode);
	MRI_REGISTER_FACTORY_METHOD(MRI::SharedFactory::InputComponentMode , MRI::ComponentMode::InputComponentRotationFourWayMode);
}