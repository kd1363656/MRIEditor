#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentInstantByMouseMode final : public MRI::ComponentMode::RotationComponentByMouseModeBase
	{
	public:

		RotationComponentInstantByMouseMode ()          = default;
		~RotationComponentInstantByMouseMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Update() override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentInstantByMouseMode , MRI::ComponentMode::RotationComponentByMouseModeBase);