#pragma once

namespace MRI::Editor
{
	class EditorLogView final : public MRI::Editor::EditorViewBase
	{
	public:
		
		EditorLogView ()          = default;
		~EditorLogView() override = default;

		std::uint32_t GetTypeID() const override { return MRI::StaticID::GetTypeID<MRI::Editor::EditorLogView>(); }

		bool IsAllowCreateInList() const override { return false; }

		void Init      () override;
		void DrawEditor() override;

		void AddLog(const char* a_format , ...) IM_FMTARGS(2);

	private:

		void DrawEditorOptions();
		void DrawLog          ();

		void ClearLog();

		static constexpr float k_filterInputWidth    = -100.0f;
		static constexpr float k_scrollToBottomRatio =  1.0f;

		static constexpr int k_nextLineStartOffset  =  1;
		static constexpr int k_excludeNewLineOffset = -1;
		static constexpr int k_nextLineIndexOffset  =  1;

		ImGuiTextBuffer m_textBuffer        = {};
		ImGuiTextFilter m_textFilter        = {};
		ImVector<int>   m_textLineOffsets   = {};
		bool            m_canAutoScroll     = false;
		bool            m_canScrollToBottom = false;
	};
}