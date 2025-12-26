#pragma once

namespace MRI::Editor
{
	class EditorHierarchyView final : public MRI::Editor::EditorViewBase
	{
	public:

		EditorHierarchyView ()          = default;
		~EditorHierarchyView() override = default;

		std::uint32_t GetTypeID() const override { return MRI::StaticID::GetTypeID<MRI::Editor::EditorHierarchyView>(); }

		void Init      () override;
		void DrawEditor() override;

		void LoadFile() override;
		void SaveFile() override;

		std::weak_ptr<MRI::GameObject> GetSelectedGameObjectCache() const { return m_selectedGameObjectCache; }

	private:

		void RunOnceSetPrevGameObjectCache();

		void DrawAddGameObjectSelector            ();
		void DrawAddGameObjectButton              ();
		void DrawHierarchy                        ();
		void UnparentIfDroppedInEmptyHierarchyArea() const;

		void RecursiveDrawHierarchy(std::weak_ptr<MRI::GameObject> a_gameObjectCache);

		void DrawHierarchyContextMenu(std::weak_ptr<MRI::GameObject> a_gameObjectCache) const;
		void DrawHierarchy           (std::weak_ptr<MRI::GameObject> a_gameObjectCache);

		void HandleHierarchyDragAndDrop(std::weak_ptr<MRI::GameObject> a_gameObjectCache);

		bool IsDescendantOf           (std::weak_ptr<MRI::GameObject> a_descendantCache , std::weak_ptr<MRI::GameObject> a_ancestorCache);
		bool IsExistSelectedGameObject();

		void SetSelectedGameObject(const std::shared_ptr<MRI::GameObject> a_gameObjectCache , const UUID& a_uuid);

		static constexpr ImVec2			  k_dropToRootAreaRect = { 0.0f , 12.0f };
		static constexpr std::string_view k_fileIOPath         = "Asset/Data/Setting/Editor/Hierarchy/HierarchySetting.json";

		std::weak_ptr<MRI::GameObject> m_selectedGameObjectCache;
		std::weak_ptr<MRI::GameObject> m_draggingGameObjectCache;

		UUID m_selectedGameObjectUUID = GUID_NULL;
		
		std::string m_createGameObjectName = MRI::CommonConstant::k_stringUnknown.data();

		bool m_isFirstTime = false;
	};
}