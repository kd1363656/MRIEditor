#pragma once

namespace MRI::Helper
{
	class ComponentRotationByMouseHelper : public MRI::Helper::ComponentHelperBase
	{
	public:

		ComponentRotationByMouseHelper ()          = default;
		~ComponentRotationByMouseHelper() override = default;

		void Init() override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

		void ToggleMouseCenterLock();

		float GetMinRotatableDegreeX() const { return m_minRotatableDegreeX; }
		float GetMaxRotatableDegreeX() const { return m_maxRotatableDegreeX; }

		bool GetIsDisableMouseCenterLock() const { return m_isDisableMouseCenterLock; }

	private:

		float m_minRotatableDegreeX = -MRI::CommonConstant::k_quarterDegree;
		float m_maxRotatableDegreeX =  MRI::CommonConstant::k_quarterDegree;

		bool m_isDisableMouseCenterLock = false;
	};
}