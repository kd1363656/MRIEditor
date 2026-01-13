#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentModeByMouseBase : public MRI::ComponentMode::RotationComponentModeBase
	{
	public:

		RotationComponentModeByMouseBase ()          = default;
		~RotationComponentModeByMouseBase() override = default;

		void Init() override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

	private:

		static constexpr float k_defaultRotationSpeed = 1.0F;

		float m_rotationSpeed = k_defaultRotationSpeed;

		float m_minRotatableDegreeX = -MRI::CommonConstant::k_quarterDegree;
		float m_maxRotatableDegreeX =  MRI::CommonConstant::k_quarterDegree;
		
		bool m_isDisableMouseCenterLock = false;

	protected:

		void ToggleMouseCenterLock();

		float GetMinRotatableDegreeX() const { return m_minRotatableDegreeX; }
		float GetMaxRotatableDegreeX() const { return m_maxRotatableDegreeX; }

		bool GetIsDisableMouseCenterLock() const { return m_isDisableMouseCenterLock; }
	};
}