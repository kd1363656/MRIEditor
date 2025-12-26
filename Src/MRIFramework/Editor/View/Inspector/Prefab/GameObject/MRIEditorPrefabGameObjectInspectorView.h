#pragma once

namespace MRI::Editor
{
	class EditorPrefabGameObjectInspectorView final : public MRI::Editor::EditorViewBase
	{
	public:

		EditorPrefabGameObjectInspectorView ()          = default;
		~EditorPrefabGameObjectInspectorView() override = default;

		std::uint32_t GetTypeID() const override { return StaticID::GetTypeID<MRI::Editor::EditorPrefabGameObjectRegistryInspectorView>(); }

		void DrawEditor() override;

	private:

		void DrawAddComponentSelector     ();
		void DrawAddComponentButton       (std::weak_ptr<MRI::GameObject> a_prefabGameObjectCache) const;
		void DrawPrefabGameObjectInspector(std::weak_ptr<MRI::GameObject> a_prefabGameObjectCache) const;

		std::weak_ptr<MRI::GameObject> GetSelectedPrefabGameObjectCache() const;

		std::string m_addComponentName = MRI::CommonConstant::k_stringUnknown.data();
	};
}