#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentLerpByMouseMode : public MRI::ComponentMode::RotationComponentLerpModeBase
	{
	public:

		RotationComponentLerpByMouseMode ()          = default;
		~RotationComponentLerpByMouseMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init() override;

		void Update() override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

	private:

		std::shared_ptr<MRI::Helper::ComponentRotationByMouseHelper> m_componentRotationByMouseHelper = nullptr;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentLerpByMouseMode , MRI::ComponentMode::RotationComponentLerpModeBase);