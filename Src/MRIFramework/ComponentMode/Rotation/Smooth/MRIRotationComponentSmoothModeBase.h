#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentSmoothModeBase : public MRI::ComponentMode::RotationComponentModeBase
	{
	public:

		RotationComponentSmoothModeBase ()          = default;
		~RotationComponentSmoothModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

		std::weak_ptr<MRI::Modifier::InterpolatorModifierBase> GetInterpolatorModifierCache() { return m_interpolatorModifier; }

	private:

		std::shared_ptr<MRI::Modifier::InterpolatorModifierBase> m_interpolatorModifier = nullptr;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentSmoothModeBase , MRI::ComponentMode::RotationComponentModeBase);