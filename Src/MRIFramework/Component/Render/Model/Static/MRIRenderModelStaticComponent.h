#pragma once

namespace MRI::Component
{
	class RenderModelStaticComponent final : public MRI::Component::RenderModelComponentBase
	{
	public:

		RenderModelStaticComponent ()          = default;
		~RenderModelStaticComponent() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init() override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		void Draw(const std::uint32_t a_shaderTag) override;

	private:

		std::shared_ptr<KdModelData> m_modelData = nullptr;
	};
}


MRI_REGISTER_TYPE_INFO(MRI::Component::RenderModelStaticComponent , MRI::Component::RenderModelComponentBase);