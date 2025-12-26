#pragma once

namespace MRI::Editor
{
	class EditorSpawnSceneShiftInspectorView final : public MRI::Editor::EditorViewBase
	{
	public:

		EditorSpawnSceneShiftInspectorView ()          = default;
		~EditorSpawnSceneShiftInspectorView() override = default;

		std::uint32_t GetTypeID() const override { return StaticID::GetTypeID<MRI::Editor::EditorSpawnSceneShiftInspectorView>(); }

		void DrawEditor() override;

	private:

		void DrawAddSceneShiftSelector();
		void DrawAddSceneShiftButton  ();

		void DrawSceneShiftMap() const;

		std::uint32_t m_addSceneShiftTag = MRI::CommonConstant::k_forbiddenStaticID;
	};
}