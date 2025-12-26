#pragma once

namespace MRI::Editor
{
	class EditorSpawnSceneLoadPrefabInspectorView final : public MRI::Editor::EditorViewBase
	{
	public:

		EditorSpawnSceneLoadPrefabInspectorView ()          = default;
		~EditorSpawnSceneLoadPrefabInspectorView() override = default;

		std::uint32_t GetTypeID() const override { return StaticID::GetTypeID<MRI::Editor::EditorSpawnSceneLoadPrefabInspectorView>(); }

		void DrawEditor() override;

		void DrawAddSceneLoadPrefabMapSelector();
		void DrawAddSceneLoadPrefabMapButton  ();
		void DrawSceneLoadPrefabMap           ();

	private:

		std::string m_addPrefabName = MRI::CommonConstant::k_stringUnknown.data();
	};
}