#pragma once

namespace MRI::Editor
{
	class EditorPrefabGameObjectRegistryInspectorView : public MRI::Editor::EditorViewBase
	{
	public:

		EditorPrefabGameObjectRegistryInspectorView ()          = default;
		~EditorPrefabGameObjectRegistryInspectorView() override = default;

		std::uint32_t GetTypeID() const override { return StaticID::GetTypeID<MRI::Editor::EditorPrefabGameObjectRegistryInspectorView>(); }

		void Init      () override;
		void DrawEditor() override;

		void LoadFile() override;
		void SaveFile() override;

		std::string FetchPrefabFilePath(const std::string& a_prefabName);

		const auto& GetAllPrefabMap() const { return m_allPrefabMap; }

	private:

		void DrawAddPrefabMapInfo();
		void DrawAllPrefabMap    ();
		
		void UpdatePopUp();

		void RegisterPrefabMap(const std::string& a_prefabName , const std::string& a_prefabFilePath);

		static constexpr std::string_view k_fileIOPath = "Asset/Data/Setting/Editor/PrefabRegistry/GameObject/GameObjectPrefabRegistrySetting.json";

		std::unordered_map<std::string , std::string , MRI::CommonStruct::StringHash , std::equal_to<>> m_allPrefabMap;

		std::unique_ptr<MRI::Editor::EditorPopup> m_saveSuccessPopup = nullptr;

		std::string m_prefabNameBuffer = MRI::CommonConstant::k_stringUnknown.data();
	};
}