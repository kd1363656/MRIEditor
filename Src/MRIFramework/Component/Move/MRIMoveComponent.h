#pragma once

namespace MRI::Component
{
	class MoveComponent final : public MRI::Component::ComponentBase
	{
	public:

		MoveComponent ()          = default;
		~MoveComponent() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void PostLoadInit() override;

		void PreUpdate() override;
		void Update   () override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

	private:

		std::unique_ptr<MRI::ComponentMode::MoveComponentModeBase> m_moveComponentMode = nullptr;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Component::MoveComponent , MRI::Component::ComponentBase);