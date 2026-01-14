#pragma once

namespace MRI::ComponentMode
{
	class RotationComponentLerpModeBase : public MRI::ComponentMode::RotationComponentModeBase
	{
	public:

		RotationComponentLerpModeBase ()          = default;
		~RotationComponentLerpModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

		std::weak_ptr<MRI::Modifier::InterpolatorModifierBase> GetInterpolatorModifierCache() const { return m_interpolatorModifier; }

	private:

		std::shared_ptr<MRI::Modifier::InterpolatorModifierBase> m_interpolatorModifier = nullptr;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentLerpModeBase , MRI::ComponentMode::RotationComponentModeBase);