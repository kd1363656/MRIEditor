#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentByMouseModeBase : public MRI::ComponentMode::RotationComponentModeBase
	{
	public:

		RotationComponentByMouseModeBase ()          = default;
		~RotationComponentByMouseModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

	protected:

		Math::Vector3 Calc();
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentByMouseModeBase , MRI::ComponentMode::RotationComponentModeBase);