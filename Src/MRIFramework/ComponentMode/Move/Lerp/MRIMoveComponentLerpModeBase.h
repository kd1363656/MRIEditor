#pragma once

namespace MRI::ComponentMode
{
	class MoveComponentLerpModeBase : public MRI::ComponentMode::MoveComponentModeBase
	{
	public:

		MoveComponentLerpModeBase ()          = default;
		~MoveComponentLerpModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

		std::weak_ptr<MRI::Modifier::InterpolatorModifierBase> GetInterpolatorModifierCache() { return m_interpolatorModifier; }

	private:

		std::shared_ptr<MRI::Modifier::InterpolatorModifierBase> m_interpolatorModifier = nullptr;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::MoveComponentLerpModeBase , MRI::ComponentMode::MoveComponentModeBase);