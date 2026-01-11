#pragma once

namespace MRI::Editor
{
	template <typename Type>
	concept IsDerivedEditorMenuBaseConcept = MRI::Concept::IsDerivedBaseConcept<Type, MRI::Editor::EditorMenuBase>;
}

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

		template <MRI::Editor::IsDerivedEditorMenuBaseConcept Type>
		void RegisterMenu()
		{
			auto l_menu = std::make_unique<Type>();
			l_menu->Init();

			m_editorMenuList.emplace_back(std::move(l_menu));
		}

		std::vector<std::unique_ptr<MRI::Editor::EditorMenuBase>> m_editorMenuList;
	};
}