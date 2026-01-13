#pragma once

namespace MRI::ComponentMode
{
	class MoveComponentLinearModeBase : public MRI::ComponentMode::MoveComponentModeBase
	{
	public:

		MoveComponentLinearModeBase ()          = default;
		~MoveComponentLinearModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init() override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

		float GetMoveSpeed() const { return m_moveSpeed; }

	private:

		static constexpr float k_defaultMoveSpeed = 1.0F;

		float m_moveSpeed = k_defaultMoveSpeed;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::MoveComponentLinearModeBase , MRI::ComponentMode::MoveComponentModeBase);