#pragma once

namespace MRI::UniqueFactory
{
	template <typename ArgumentType>
	using Strategy = MRI::GenericFactory<std::unique_ptr<ArgumentType>>;

	using RotationComponentMode = MRI::GenericFactory<std::unique_ptr<MRI::ComponentMode::RotationComponentModeBase>>;
	using MoveComponentMode     = MRI::GenericFactory<std::unique_ptr<MRI::ComponentMode::MoveComponentModeBase>>;
}