#pragma once

namespace MRI
{
	class RenderManager final : public MRI::SingletonBase<MRI::RenderManager>
	{
	public:

		void Init        ();
		void PostLoadInit();

		void BeginDraw  (const bool a_usePostProcess = true) const;
		void PreDraw    ()                                   const;
		void Draw       ()                                   const;
		void PostDraw   ()                                   const;
		void DrawSprite ()                                   const;
		void DrawDebug  ()									 const;
		void EndDraw    ()                                   const;

		void AddRenderModelComponentBase(std::weak_ptr<MRI::Component::RenderModelComponentBase> a_componentCache);

		bool HasAlreadyRenderModelComponentBaseCache(const std::weak_ptr<MRI::Component::RenderModelComponentBase> a_renderModelComponentBaseCache);

		void SetMainCameraComponentCache(const std::shared_ptr<MRI::Component::CameraComponent> a_cameraComponentCache) { m_mainCameraComponentCache = a_cameraComponentCache; }

	private:

		void Reset();

		template <typename Type>
		void DrawModel() const
		{
			for (const auto& l_renderModelComponentBaseCache : m_renderModelComponentBaseCacheList)
			{
				auto l_renderModelComponentBase = l_renderModelComponentBaseCache.lock();
				if (!l_renderModelComponentBase) { continue; }

				const std::uint32_t l_shaderTag = MRI::GetTypeInfo<Type>().k_id;
				l_renderModelComponentBase->Draw(l_shaderTag);
			}
		}

		std::weak_ptr<MRI::Component::CameraComponent> m_mainCameraComponentCache;
		
		std::list<std::weak_ptr<MRI::Component::RenderModelComponentBase>> m_renderModelComponentBaseCacheList;

		//===============================
		// シングルトン
		//===============================
		friend class MRI::SingletonBase<MRI::RenderManager>;

		RenderManager ()          = default;
		~RenderManager() override = default;
	};
}