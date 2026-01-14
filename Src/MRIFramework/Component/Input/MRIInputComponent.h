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

		void EditPrefabInspector() override;
	
		void DeserializePrefab(const nlohmann::json& a_json) override;
	
		nlohmann::json SerializePrefab() override;

	private:

		bool IsAlreadyContains(const std::shared_ptr<MRI::ComponentMode::InputComponentModeBase> a_addComponentMode);

		void EditAddInputComponentModeList();

		std::vector<std::shared_ptr<MRI::ComponentMode::InputComponentModeBase>> m_inputComponentModeList;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Component::InputComponent , MRI::Component::ComponentBase);