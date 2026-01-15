#pragma once

namespace MRI::ComponentMode
{
	class InputComponentMoveFourWayMode final : public MRI::ComponentMode::InputComponentMoveModeBase
	{
	public:

		InputComponentMoveFourWayMode ()          = default;
		~InputComponentMoveFourWayMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void EarlyUpdate() override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::InputComponentMoveFourWayMode , MRI::ComponentMode::InputComponentMoveModeBase);