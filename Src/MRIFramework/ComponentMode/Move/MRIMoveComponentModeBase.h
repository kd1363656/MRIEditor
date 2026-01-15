#pragma once

namespace MRI::ComponentMode
{
	class MoveComponentModeBase : public MRI::ComponentMode::ComponentModeBase
	{
	public:

		MoveComponentModeBase ()          = default;
		~MoveComponentModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init        ()											 override;
		void PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner) override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

		void ResetMoveDirection();

		void RegisterAdaptMoveAxisTag(const std::uint32_t a_addMoveAxisAdaptTag);

		const auto& GetSelfTransformComponentCache() const { return m_selfTransformComponentCache; }

		Math::Vector3& GetWorkMoveDirection() { return m_moveDirection; }

		void SetMoveDirection(const Math::Vector3& a_set) { m_moveDirection = a_set; }

	private:

		void EditAddAdaptMoveDirectionTag();

		std::unordered_set<std::uint32_t> m_adaptMoveAxisTagSet;

		std::weak_ptr<MRI::Component::TransformComponent> m_selfTransformComponentCache;

		Math::Vector3 m_moveDirection = Math::Vector3::Zero;

		std::uint32_t m_debugAddMoveAxisAdaptTag = MRI::CommonConstant::k_invalidStaticID;

	protected:

		virtual Math::Vector3 CalcMoveDirection();

		const Math::Vector3& GetMoveDirection() const { return m_moveDirection; }

		const auto& GetWorkAdaptMoveAxisTagSet() const { return m_adaptMoveAxisTagSet; }
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::MoveComponentModeBase , MRI::ComponentMode::ComponentModeBase);