#pragma once

namespace MRI::Editor
{
	class EditorPlayerKeyConfigInspectorView final : public MRI::Editor::EditorViewBase
	{
	public:

		EditorPlayerKeyConfigInspectorView ()          = default;
		~EditorPlayerKeyConfigInspectorView() override = default;

		std::uint32_t GetTypeID() const override { return StaticID::GetTypeID<MRI::Editor::EditorPlayerKeyConfigInspectorView>(); }

		void DrawEditor() override;

	private:
	
		void DrawAddKeyConfigInfo ();
		void DrawAddVKCodeSelector();
		void DrawKeyConfigMap     () const;

		std::uint32_t m_addInputEventTag = MRI::CommonConstant::k_forbiddenStaticID;
		int			  m_addVKCode        = NULL;
	};
}