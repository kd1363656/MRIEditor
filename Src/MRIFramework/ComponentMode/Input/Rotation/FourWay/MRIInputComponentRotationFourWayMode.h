#pragma once

namespace MRI::ComponentMode
{
	class InputComponentRotationFourWayMode final : public MRI::ComponentMode::InputComponentRotationModeBase
	{
	public:

		InputComponentRotationFourWayMode()           = default;
		~InputComponentRotationFourWayMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void EarlyUpdate() override;
		
	private:

		std::weak_ptr<MRI::Component::RotationComponent> m_selfRotationComponentCache;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::InputComponentRotationFourWayMode , MRI::ComponentMode::InputComponentRotationModeBase);