#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentByMouseModeBase : public MRI::ComponentMode::RotationComponentModeBase
	{
	public:

		RotationComponentByMouseModeBase ()          = default;
		~RotationComponentByMouseModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init() override;

		void Update() override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

	private:

		static constexpr float k_defaultRotationSpeed = 1.0F;

		Math::Vector3 m_targetRotation = Math::Vector3::Zero;

		float m_rotationSpeed       =  k_defaultRotationSpeed;
		float m_minRotatableDegreeX = -MRI::CommonConstant::k_quarterDegree;
		float m_maxRotatableDegreeX =  MRI::CommonConstant::k_quarterDegree;
		
		bool m_disableMouseLock = false;
		
	protected:

		void MouseLock();

		const Math::Vector3& GetTargetRotation() const { return m_targetRotation; }

		float GetRotationSpeed() const { return m_rotationSpeed; }

		bool GetDisableMouseLock() const { return m_disableMouseLock; }
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentByMouseModeBase , MRI::ComponentMode::RotationComponentModeBase);