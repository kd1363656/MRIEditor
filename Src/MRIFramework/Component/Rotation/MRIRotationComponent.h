#pragma once

namespace MRI::Component
{
	class RotationComponent final : public MRI::Component::ComponentBase
	{
	public:

		RotationComponent ()          = default;
		~RotationComponent() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

	private:

	};
}

MRI_REGISTER_TYPE_INFO(MRI::Component::RotationComponent , MRI::Component::ComponentBase);