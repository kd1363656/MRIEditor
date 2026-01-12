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

	private:

		std::shared_ptr<MRI::Modifier::InterpolatorModifierBase> m_interpolatorModifierBase = nullptr;

	protected:

		std::weak_ptr<MRI::Modifier::InterpolatorModifierBase> GetInterpolatorModifier() const { return m_interpolatorModifierBase; }
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::MoveComponentLerpModeBase , MRI::ComponentMode::MoveComponentModeBase);