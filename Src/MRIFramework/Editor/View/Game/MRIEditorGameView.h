#pragma once

namespace MRI::Editor
{
	class EditorGameView final : public MRI::Editor::EditorViewBase
	{
	public:

		EditorGameView ()          = default;
		~EditorGameView() override = default;

		std::uint32_t GetTypeID() const override { return MRI::StaticID::GetTypeID<MRI::Editor::EditorGameView>(); }

		void Init      () override;
		void DrawEditor() override;

		void RenderTargetClearTex();
		void ChangeRenderTarget  ();
		void UndoRenderTarget    ();

		KdRenderTargetChanger& GetWorkRenderTargetChanger() { return m_renderTargetChanger; }
		KdRenderTargetPack&    GetWorkRenderTargetPack   () { return m_renderTargetPack;    }

	private:

		void DrawSceneSelector();
		void DrawScene	      ();

		static constexpr ImVec2 k_generalAspectRation = { 16.0F , 9.0F };

		KdRenderTargetChanger m_renderTargetChanger;
		KdRenderTargetPack    m_renderTargetPack;

		ImVec2 m_nowSceneImageSize = {};
	};
}