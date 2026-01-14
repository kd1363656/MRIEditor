#pragma once

namespace MRI::ComponentMode
{
	class InputComponentModeBase : public MRI::ComponentMode::ComponentModeBase
	{
	public:

		InputComponentModeBase ()          = default;
		~InputComponentModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::InputComponentModeBase , MRI::ComponentMode::ComponentModeBase);