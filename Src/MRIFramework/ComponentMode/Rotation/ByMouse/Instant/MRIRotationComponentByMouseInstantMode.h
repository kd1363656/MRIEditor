#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentByMouseInstantMode : public MRI::ComponentMode::RotationComponentByMouseModeBase
	{
	public:

		RotationComponentByMouseInstantMode ()          = default;
		~RotationComponentByMouseInstantMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Update() override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentByMouseInstantMode , MRI::ComponentMode::RotationComponentByMouseModeBase);