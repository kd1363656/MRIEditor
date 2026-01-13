#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentSmoothByMouseMode : public MRI::ComponentMode::RotationComponentSmoothModeBase
	{
	public:

		RotationComponentSmoothByMouseMode ()          = default;
		~RotationComponentSmoothByMouseMode() override = default;

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

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentSmoothByMouseMode , MRI::ComponentMode::RotationComponentSmoothModeBase);