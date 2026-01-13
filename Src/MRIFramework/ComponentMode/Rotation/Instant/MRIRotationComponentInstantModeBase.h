#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentInstantModeBase : public MRI::ComponentMode::RotationComponentModeBase
	{
	public:

		RotationComponentInstantModeBase ()          = default;
		~RotationComponentInstantModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init() override;
		
		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

	private:

		static constexpr float k_defaultRotationSpeed = 1.0F;

		float m_rotationSpeed = k_defaultRotationSpeed;

	protected:

		float GetRotationSpeed() const { return m_rotationSpeed; }
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentInstantModeBase , MRI::ComponentMode::RotationComponentModeBase);