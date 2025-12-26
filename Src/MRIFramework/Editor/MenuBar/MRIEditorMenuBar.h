#pragma once

namespace MRI::Editor
{
	class EditorMenuBar final
	{
	public:

		EditorMenuBar () = default;
		~EditorMenuBar() = default;

		void Init      ();
		void DrawEditor() const;

	private:

		void RegisterMenu(std::unique_ptr<MRI::Editor::EditorMenuBase> a_menu);

		std::vector<std::unique_ptr<MRI::Editor::EditorMenuBase>> m_editorMenuList;
	};
}