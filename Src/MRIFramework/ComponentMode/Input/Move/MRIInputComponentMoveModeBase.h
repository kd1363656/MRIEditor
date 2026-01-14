#pragma once

namespace MRI::Component
{
	class MoveComponent;
}

namespace MRI::ComponentMode
{
	class InputComponentMoveModeBase : public MRI::ComponentMode::InputComponentModeBase
	{
	public:

		InputComponentMoveModeBase ()          = default;
		~InputComponentMoveModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner) override;

		auto& GetWorkSelfMoveComponentCache() { return m_selfMoveComponentCache; }

	private:

		std::weak_ptr<MRI::Component::MoveComponent> m_selfMoveComponentCache;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::InputComponentMoveModeBase , MRI::ComponentMode::InputComponentModeBase);