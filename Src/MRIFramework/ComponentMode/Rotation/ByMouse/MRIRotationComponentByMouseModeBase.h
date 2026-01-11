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

		static constexpr float k_defaultMaxRotationSpeed = 1.0F;

		std::shared_ptr<MRI::Modifier::InterpolatorModifierBase> m_interpolatorModifier = nullptr;

		Math::Vector3 m_targetRotation = Math::Vector3::Zero;

		float m_minRotatableDegreeX = -MRI::CommonConstant::k_quarterDegree;
		float m_maxRotatableDegreeX =  MRI::CommonConstant::k_quarterDegree;
		
		bool m_disableMouseLock = false;
		bool m_isMouseMove      = false;

	protected:

		void MouseLock();

		std::weak_ptr<MRI::Modifier::InterpolatorModifierBase> GetInterpolatorModifierCache() const { return m_interpolatorModifier; }

		const Math::Vector3& GetTargetRotation() const { return m_targetRotation; }

		bool GetDisableMouseLock() const { return m_disableMouseLock; }
		bool GetIsMouseMove     () const { return m_isMouseMove;      }
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentByMouseModeBase , MRI::ComponentMode::RotationComponentModeBase);