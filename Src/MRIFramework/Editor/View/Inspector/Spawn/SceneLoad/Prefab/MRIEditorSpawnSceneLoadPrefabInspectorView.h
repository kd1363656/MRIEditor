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
		void DrawAddSceneLoadPrefabMapButton  () const;
		void DrawSceneLoadPrefabMap           () const;

	private:

		std::string m_addPrefabName = std::string(MRI::CommonConstant::k_stringUnknown.data());
	};
}