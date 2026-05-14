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

		void SetupMainCamera(const std::shared_ptr<GameObject>& a_gameObject);

		std::weak_ptr<MRI::GameObject> GetSelectedGameObjectCache() const { return m_selectedGameObjectCache; }

	private:

		void RunOnceSetPrevGameObjectCache();

		void SetUUID(const std::shared_ptr<MRI::GameObject> a_gameObject);

		void DrawAddGameObjectSelector       ();
		void DrawAddGameObjectButton         ();
		void DrawGameObjectHierarchy         ();
		void RecursiveDrawGameObjectHierarchy(const std::shared_ptr<MRI::GameObject>& a_gameObject);

		void HandleHierarchyDragAndDrop(const std::weak_ptr<MRI::GameObject> a_gameObjectCache);
		void HandleContextMenu         (const std::weak_ptr<MRI::GameObject> a_gameObjectCache) const;

		void UnparentIfDroppedInEmptyHierarchyArea() const;

		bool IsDescendant(const std::weak_ptr<MRI::GameObject> a_parentCache , const std::weak_ptr<MRI::GameObject> a_targetCache);

		void SetSelectedGameObjectCache(const std::weak_ptr<MRI::GameObject> a_gameObjectCache);

		void SetupPrefabCache(const std::shared_ptr<MRI::GameObject> a_gameObject);

		static constexpr ImVec4 k_prefabGameObjectColor = ImVec4(0.30F , 1.0F , 0.30F , 1.0F);

		const std::string k_fileIOPath = "Asset/Data/Setting/Editor/Hierarchy/HierarchySetting.json";

		std::weak_ptr<MRI::GameObject> m_selectedGameObjectCache;
		std::weak_ptr<MRI::GameObject> m_draggingGameObjectCache;

		UUID m_selectedGameObjectUUID = GUID_NULL;

		std::string m_createGameObjectName = MRI::CommonConstant::k_stringUnknown.data();

		bool m_isFirstTime = false;
	};
}