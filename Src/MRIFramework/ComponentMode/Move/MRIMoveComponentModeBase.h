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

		void PreUpdate() override;

		const auto& GetSelfTransformComponentCache() const { return m_selfTransformComponentCache; }

		const Math::Vector3& GetMoveDirection    () const { return m_moveDirection; }
		Math::Vector3&       GetWorkMoveDirection()		  { return m_moveDirection; }

		void SetMoveDirection(const Math::Vector3& a_set) { m_moveDirection = a_set; }

	private:

		std::weak_ptr<MRI::Component::TransformComponent> m_selfTransformComponentCache;

		Math::Vector3 m_moveDirection = Math::Vector3::Zero;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::MoveComponentModeBase , MRI::ComponentMode::ComponentModeBase);