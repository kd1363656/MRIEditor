#pragma once

namespace MRI::Helper
{
	class RotationComponentByMouseHelper : public MRI::Helper::ComponentHelperBase
	{
	public:

		RotationComponentByMouseHelper ()          = default;
		~RotationComponentByMouseHelper() override = default;

		void Init() override;

		void ToggleMouseCenterLock();

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

		float GetMinRotatableDegreeX() const { return m_minRotatableDegreeX; }
		float GetMaxRotatableDegreeX() const { return m_maxRotatableDegreeX; }

		bool GetIsDisableMouseCenterLock() const { return m_isDisableMouseCenterLock; }

	private:

		static constexpr float k_defaultRotationSpeed = 1.0F;

		float m_minRotatableDegreeX = -MRI::CommonConstant::k_quarterDegree;
		float m_maxRotatableDegreeX =  MRI::CommonConstant::k_quarterDegree;
		
		bool m_isDisableMouseCenterLock = false;
	};
}