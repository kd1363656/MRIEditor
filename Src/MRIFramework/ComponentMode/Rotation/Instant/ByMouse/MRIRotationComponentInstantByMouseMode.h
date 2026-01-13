#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentInstantByMouseMode final : public MRI::ComponentMode::RotationComponentInstantModeBase
	{
	public:

		RotationComponentInstantByMouseMode ()          = default;
		~RotationComponentInstantByMouseMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init() override;

		void Update() override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

	private:

		std::shared_ptr<MRI::Helper::RotationComponentByMouseHelper> m_rotationComponentByMouseHelper = nullptr;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentInstantByMouseMode , MRI::ComponentMode::RotationComponentInstantModeBase);