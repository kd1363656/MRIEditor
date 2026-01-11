#pragma once

namespace MRI::Component
{
	class MoveComponent final : public MRI::Component::ComponentBase
	{
	public:

		MoveComponent ()          = default;
		~MoveComponent() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

	private:

	};
}

MRI_REGISTER_TYPE_INFO(MRI::Component::MoveComponent , MRI::Component::ComponentBase);