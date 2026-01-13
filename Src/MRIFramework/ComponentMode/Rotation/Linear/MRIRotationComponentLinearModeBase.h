#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentLinearModeBase : public MRI::ComponentMode::RotationComponentModeBase
	{
	public:

		RotationComponentLinearModeBase ()          = default;
		~RotationComponentLinearModeBase() override = default;
		
		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init() override;
		
		void EditPrefabInspector() override;
	
		void DeserializePrefab(const nlohmann::json& a_json) override;
	
		nlohmann::json SerializePrefab() override;

		float GetRotationSpeed() const { return m_rotationSpeed; }

	private:

		static constexpr float k_defaultRotationSpeed = 1.0F;

		float m_rotationSpeed = k_defaultRotationSpeed;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentLinearModeBase , MRI::ComponentMode::RotationComponentModeBase);