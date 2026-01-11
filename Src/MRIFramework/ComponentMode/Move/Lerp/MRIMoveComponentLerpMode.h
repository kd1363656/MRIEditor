#pragma once

namespace MRI::ComponentMode
{
	class MoveComponentLerpMode : public MRI::ComponentMode::MoveComponentModeBase
	{
	public:

		MoveComponentLerpMode ()          = default;
		~MoveComponentLerpMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::MoveComponentLerpMode , MRI::ComponentMode::MoveComponentModeBase);