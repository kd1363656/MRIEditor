#pragma once

namespace MRI::Editor
{
	class EditorPrefabSceneShiftRegistryInspectorView final : public MRI::Editor::EditorViewBase
	{
	public:

		EditorPrefabSceneShiftRegistryInspectorView ()          = default;
		~EditorPrefabSceneShiftRegistryInspectorView() override = default;

		std::uint32_t GetTypeID() const override { return StaticID::GetTypeID<MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView>(); }

		void DrawEditor() override;

		void LoadFile() override;
		void SaveFile() override;

		std::string FetchSceneShiftFilePath(const std::uint32_t a_tag) const;

		const auto& GetAllSceneShiftMap() const { return m_allSceneShiftMap; }

	private:

		static constexpr std::string_view k_fileIOPath = "Asset/Data/Setting/Editor/PrefabRegistry/SceneShift/PrefabSceneShiftRegistrySetting.json";

		void DrawAddSceneShiftInfo();
		void DrawAllSceneShiftMap ();

		void RegisterSceneShiftMap(const uint32_t a_tag , const std::string& a_filePath);

		std::unordered_map<std::uint32_t , std::string> m_allSceneShiftMap;

		std::uint32_t m_addSceneShiftTag = MRI::CommonConstant::k_forbiddenStaticID;
	};
}