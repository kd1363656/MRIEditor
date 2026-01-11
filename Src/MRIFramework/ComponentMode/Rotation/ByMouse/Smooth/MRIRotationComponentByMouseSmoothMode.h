#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentByMouseSmoothMode : public MRI::ComponentMode::RotationComponentByMouseModeBase
	{
	public:

		RotationComponentByMouseSmoothMode ()          = default;
		~RotationComponentByMouseSmoothMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Update() override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentByMouseSmoothMode , MRI::ComponentMode::RotationComponentByMouseModeBase);