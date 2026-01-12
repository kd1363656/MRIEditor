#pragma once

namespace MRI::ComponentMode
{
	class MoveComponentLerpFacingDirectionMode final : public MRI::ComponentMode::MoveComponentLerpModeBase
	{
	public:

		MoveComponentLerpFacingDirectionMode ()          = default;
		~MoveComponentLerpFacingDirectionMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Update() override;

	private:



	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::MoveComponentLerpFacingDirectionMode , MRI::ComponentMode::MoveComponentLerpModeBase);