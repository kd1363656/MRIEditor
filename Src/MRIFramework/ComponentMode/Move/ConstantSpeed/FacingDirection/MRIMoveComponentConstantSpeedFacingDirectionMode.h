#pragma once

namespace MRI::ComponentMode
{
	class MoveComponentConstantSpeedFacingDirectionMode final : public MRI::ComponentMode::MoveComponentConstantSpeedModeBase
	{
	public:

		MoveComponentConstantSpeedFacingDirectionMode ()          = default;
		~MoveComponentConstantSpeedFacingDirectionMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void PreUpdate() override;
		void Update   () override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::MoveComponentConstantSpeedFacingDirectionMode , MRI::ComponentMode::MoveComponentModeBase);