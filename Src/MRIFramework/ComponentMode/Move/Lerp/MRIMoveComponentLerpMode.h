#pragma once

namespace MRI::ComponentMode
{
	class MoveComponentLerpMode : public MRI::ComponentMode::MoveComponentModeBase
	{
	public:

		MoveComponentLerpMode ()          = default;
		~MoveComponentLerpMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;
		
		nlohmann::json SerializePrefab() override;

	private:
		
		std::shared_ptr<MRI::Modifier::InterpolatorModifierBase> m_interpolatorModifier = nullptr;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::MoveComponentLerpMode , MRI::ComponentMode::MoveComponentModeBase);