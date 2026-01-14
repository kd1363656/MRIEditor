#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentLerpTargetViewDirectionMode : public MRI::ComponentMode::RotationComponentLerpModeBase
	{
	public:

		RotationComponentLerpTargetViewDirectionMode ()          = default;
		~RotationComponentLerpTargetViewDirectionMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentLerpTargetViewDirectionMode , MRI::ComponentMode::RotationComponentLerpModeBase);