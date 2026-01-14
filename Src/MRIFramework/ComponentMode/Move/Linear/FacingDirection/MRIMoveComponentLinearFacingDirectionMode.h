#pragma once

namespace MRI::ComponentMode
{
	class MoveComponentLinearFacingDirectionMode final : public MRI::ComponentMode::MoveComponentLinearModeBase
	{
	public:

		MoveComponentLinearFacingDirectionMode ()          = default;
		~MoveComponentLinearFacingDirectionMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void PreUpdate() override;
		void Update   () override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::MoveComponentLinearFacingDirectionMode , MRI::ComponentMode::MoveComponentModeBase);