#pragma once

namespace MRI::ComponentMode
{
	class MoveComponentLerpTargetViewDirectionMode final : public MRI::ComponentMode::MoveComponentLerpModeBase
	{
	public:

		MoveComponentLerpTargetViewDirectionMode ()          = default;
		~MoveComponentLerpTargetViewDirectionMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init()											 override;
		void PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner) override;

		void PreUpdate() override;
		void Update() override;

		void EditSpawnInspector() override;

		void DeserializeSpawn(const nlohmann::json& a_json) override;

		nlohmann::json SerializeSpawn() override;

	private:

		std::shared_ptr<MRI::Helper::ComponentUUIDFetcherHelper<MRI::Component::TransformComponent>> m_targetTransformComponentFetcher = nullptr;

	protected:

		Math::Vector3 CalcMoveDirection() override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::MoveComponentLerpTargetViewDirectionMode , MRI::ComponentMode::MoveComponentLerpModeBase);