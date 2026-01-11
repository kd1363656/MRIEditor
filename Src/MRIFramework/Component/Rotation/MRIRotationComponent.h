#pragma once

namespace MRI::Component
{
	class RotationComponent final : public MRI::Component::ComponentBase
	{
	public:

		RotationComponent ()          = default;
		~RotationComponent() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void PostLoadInit() override;

		void PreUpdate() override;
		void Update   () override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

	private:

		std::unique_ptr<MRI::ComponentMode::RotationComponentModeBase> m_rotationComponentMode = nullptr;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Component::RotationComponent , MRI::Component::ComponentBase);