#pragma once

namespace MRI
{
	class PlayerKeyConfig;
}

namespace MRI::Component
{
	class InputComponent final : public MRI::Component::ComponentBase
	{
	public:

		InputComponent ()          = default;
		~InputComponent() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init        () override;
		void PostLoadInit() override;

		void EarlyUpdate() override;

	private:
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Component::InputComponent , MRI::Component::ComponentBase);