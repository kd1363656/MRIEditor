#pragma once

namespace MRI::Modifier
{
	class InterpolatorModifierBase
	{
	public:

		InterpolatorModifierBase         () = default;
		virtual ~InterpolatorModifierBase() = default;

		virtual const MRI::TypeInfo& GetTypeInfo() const;

		virtual void Init();

		virtual void Update() = 0;

		virtual void EditPrefabInspector();

		virtual void DeserializePrefab(const nlohmann::json& Json);

		virtual nlohmann::json SerializePrefab();

		virtual void ResetInterpolate();

		float GetCurrentValue() const { return m_currentValue; }

	private:

		static constexpr float k_maxMeasurementTime = 1.0F;
		static constexpr float k_maxClampValue      = 1.0F;

		float m_progress = 0.0F;

		float m_maxMeasurementTime = k_maxMeasurementTime;
		float m_elapsedTime        = 0.0f;

		float m_maxValue     = k_maxClampValue;
		float m_currentValue = 0.0F;

	protected:

		float GetElapsedTime() const { return m_elapsedTime; }

		float GetMaxMeasurementTime() const { return m_maxMeasurementTime; }
		float GetMaxValue		   () const { return m_maxValue;		   }

		void SetProgress    (const float a_set) { m_progress     = a_set; }
		void SetElapsedTime (const float a_set) { m_elapsedTime  = a_set; }
		void SetCurrentValue(const float a_set) { m_currentValue = a_set; }
	};
}

MRI_REGISTER_TYPE_INFO_ROOT(MRI::Modifier::InterpolatorModifierBase);