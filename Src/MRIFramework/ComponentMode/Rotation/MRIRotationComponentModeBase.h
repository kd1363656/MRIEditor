#pragma once

namespace MRI::Component
{
	class TransformComponent;
}

namespace MRI::ComponentMode
{
	class RotationComponentModeBase : public MRI::ComponentMode::ComponentModeBase
	{
	public:

		RotationComponentModeBase ()          = default;
		~RotationComponentModeBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init        ()											 override;
		void PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner) override;

		void EditPrefabInspector() override;
	
		void DeserializePrefab(const nlohmann::json& a_json) override;
	
		nlohmann::json SerializePrefab() override;

		void RegisterAdaptRotationDirectionTag(const std::uint32_t a_addRotationDirectionAdaptTag);

	private:

		void EditAddAdaptRotationDirectionTag();

		std::unordered_set<std::uint32_t> m_adaptRotationDirectionTagList;

		std::weak_ptr<MRI::Component::TransformComponent> m_selfTransformComponentCache;

		Math::Vector3 m_rotationDirection = Math::Vector3::Zero;

		std::uint32_t m_debugAddRotationDirectionAdaptTag = MRI::CommonConstant::k_invalidStaticID;

	protected:

		const auto& GetWorkAdaptRotationDirectionTagList() const { return m_adaptRotationDirectionTagList; }

		auto& GetWorkSelfTransformComponentCache() const { return m_selfTransformComponentCache; }

		const auto& GetRotationDirection() const { return m_rotationDirection; }

		void SetRotationDirection(const Math::Vector3& a_set) { m_rotationDirection = a_set; }
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::RotationComponentModeBase , MRI::ComponentMode::ComponentModeBase);