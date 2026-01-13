#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentSmoothByMouseMode final : public MRI::ComponentMode::RotationComponentByMouseModeBase
	{
	public:

		RotationComponentSmoothByMouseMode ()          = default;
		~RotationComponentSmoothByMouseMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Update() override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentSmoothByMouseMode , MRI::ComponentMode::RotationComponentByMouseModeBase);