#pragma once

namespace MRI::Component
{
	class RotationComponent;
}

namespace MRI::ComponentMode
{
	class InputComponentRotationModeBase : public MRI::ComponentMode::InputComponentModeBase
	{
	public:

		InputComponentRotationModeBase ()          = default;
		~InputComponentRotationModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner) override;

		auto& GetWorkSelfRotationComponentCache() { return m_selfRotationComponentCache; }

	private:

		std::weak_ptr<MRI::Component::RotationComponent> m_selfRotationComponentCache;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::InputComponentRotationModeBase , MRI::ComponentMode::InputComponentModeBase);