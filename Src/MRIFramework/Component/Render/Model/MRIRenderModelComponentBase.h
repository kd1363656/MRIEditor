#pragma once

namespace MRI::Component
{
	class RenderModelComponentBase : public MRI::Component::ComponentBase
	{
	public:

		RenderModelComponentBase ()          = default;
		~RenderModelComponentBase() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init() override;

		void PostLoadInit() override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

		virtual void Draw(const std::uint32_t a_shaderTag) = 0;

	private:

		std::weak_ptr<MRI::Component::TransformComponent> m_selfTransformComponentCache = {};

		std::unordered_set<std::uint32_t> m_shaderTagSet;

		std::shared_ptr<MRI::Helper::AssetFilePathHelper> m_assetFilePathHelper = nullptr;

		Math::Color m_color = MRI::CommonConstant::k_colorLess;

		std::uint32_t m_debugAddSelectedTag = MRI::CommonConstant::k_invalidStaticID;

	protected:

		void RegisterShaderTag(const std::uint32_t a_shaderTag);

		bool CanUseShader(const std::uint32_t a_shaderTag) const;

		std::weak_ptr<MRI::Helper::AssetFilePathHelper> GetAssetFilePathHelperCache() const { return m_assetFilePathHelper; }

		std::weak_ptr<MRI::Component::TransformComponent> GetSelfTransformComponentCache() const { return m_selfTransformComponentCache; }

		const Math::Color& GetColor() const { return m_color; }
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Component::RenderModelComponentBase , MRI::Component::ComponentBase);