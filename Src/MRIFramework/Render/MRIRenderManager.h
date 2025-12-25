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

	private:

		//===============================
		// シングルトン
		//===============================
		friend class MRI::SingletonBase<MRI::RenderManager>;

		RenderManager ()          = default;
		~RenderManager() override = default;
	};
}