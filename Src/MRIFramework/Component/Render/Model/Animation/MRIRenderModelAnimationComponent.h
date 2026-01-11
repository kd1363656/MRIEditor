#pragma once

namespace MRI::Component
{
	class RenderModelAnimationComponent final : public MRI::Component::RenderModelComponentBase
	{
	public:

		RenderModelAnimationComponent ()          = default;
		~RenderModelAnimationComponent() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		void Draw(const std::uint32_t a_shaderTag) override;

	private:

		std::shared_ptr<KdModelWork> m_modelWork = nullptr;
		std::unique_ptr<KdAnimator>  m_animator  = nullptr;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Component::RenderModelAnimationComponent , MRI::Component::RenderModelComponentBase);