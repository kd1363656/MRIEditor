#pragma once

namespace MRI::Component
{
	class CameraComponent final : public MRI::Component::ComponentBase
	{
	public:

		CameraComponent ()          = default;
		~CameraComponent() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init        () override;
		void PostLoadInit() override;

		void EditSpawnInspector () override;
		void EditPrefabInspector() override;

		void DeserializeSpawn (const nlohmann::json& a_json) override;
		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializeSpawn () override;
		nlohmann::json SerializePrefab() override;

		void PreDraw();

		std::weak_ptr<KdCamera> GetCameraCache() const { return m_camera; }

		std::uint32_t GetCameraTag() const { return m_cameraTag; }

		void SetDebugCameraTag();

	private:

		void EditEnableMainCamera ();
		void EditDisableMainCamera();

		void CreateFrustum();

		static constexpr float k_defaultFieldOfView = 45.0F;
		static constexpr float k_minFieldOfView     = 1.0F;
		static constexpr float k_maxFieldOfView     = 89.0F;
		
		std::shared_ptr<KdCamera> m_camera = nullptr;

		std::weak_ptr<MRI::Component::TransformComponent> m_selfTransformComponentCache;

		DirectX::BoundingFrustum m_boundingFrustum = {};

		float m_fieldOfView = k_defaultFieldOfView;
		
		std::uint32_t m_cameraTag = MRI::CommonConstant::k_invalidStaticID;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Component::CameraComponent , MRI::Component::ComponentBase);