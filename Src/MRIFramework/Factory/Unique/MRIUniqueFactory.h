#pragma once

namespace MRI::UniqueFactory
{
	template <typename ArgumentType>
	using Strategy = MRI::GenericFactory<std::unique_ptr<ArgumentType>>;

	using ComponentMode = MRI::GenericFactory<std::unique_ptr<MRI::ComponentMode::ComponentModeBase>>;
}