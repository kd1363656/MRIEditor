#pragma once

namespace MRI::SharedFactory
{
	using Component             = MRI::GenericFactory<std::shared_ptr<MRI::Component::ComponentBase>>;
	using ObserverNotifier      = MRI::GenericFactory<std::shared_ptr<MRI::Observer::ObserverNotifierBase>>;
	using InterpolatorModifier  = MRI::GenericFactory<std::shared_ptr<MRI::Modifier::InterpolatorModifierBase>>;
	using RotationComponentMode = MRI::GenericFactory<std::shared_ptr<MRI::ComponentMode::RotationComponentModeBase>>;
	using MoveComponentMode     = MRI::GenericFactory<std::shared_ptr<MRI::ComponentMode::MoveComponentModeBase>>;
	using InputComponentMode    = MRI::GenericFactory<std::shared_ptr<MRI::ComponentMode::InputComponentModeBase>>;
}