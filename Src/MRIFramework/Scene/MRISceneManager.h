#pragma once

namespace MRI
{
	class PlayerKeyConfig;

	namespace FileIO 
	{
		class SceneManagerFileIO;
	}

	namespace Converter
	{
		class SceneManagerConverter;
	}
}

namespace MRI
{
	class SceneManager final : public MRI::SingletonBase<MRI::SceneManager>
	{
	public:

		void Init        ();
		void PostLoadInit() const;

		void LoadFile();
		void SaveFile() const;

		void SceneShift();
		void SceneShift(const std::string& a_filePath);
	
		void EarlyUpdate();
		void PreUpdate  ();
		void Update     () const;
		void LateUpdate () const;
		void FixMatrix  ();

		void           Deserialize(const nlohmann::json& a_json);
		nlohmann::json Serialize  ();

		void RegisterSceneShiftMap(const std::uint32_t a_tag , const std::string& a_sceneFilePath);
		void ClearSceneShiftMap   ();

		std::string FetchSceneShiftLoadFilePath(const std::uint32_t a_tag);

		std::weak_ptr<MRI::Scene>           GetSceneCache          () { return m_scene; }
		std::weak_ptr<MRI::PlayerKeyConfig> GetPlayerKeyConfigCache() { return m_playerKeyConfig; }

		const auto& GetSceneShiftMap    () const { return m_sceneShiftMap; }
		auto&       GetWorkSceneShiftMap()       { return m_sceneShiftMap; }

		void SetSceneManagerFileIOPath(const std::string& a_set);

		void SetNextSceneTag(const std::uint32_t a_set) { m_nextSceneTag = a_set; }

	private:
	
		static constexpr std::string_view k_playerKeyConfigIOPath = "Asset/Data/Setting/PlayerKeyConfig/PlayerKeyConfig.json";

		std::unordered_map<std::uint32_t , std::string> m_sceneShiftMap;

		std::shared_ptr<MRI::Scene>			  m_scene           = nullptr;
		std::shared_ptr<MRI::PlayerKeyConfig> m_playerKeyConfig = nullptr;
 
		std::unique_ptr<MRI::FileIO::SceneManagerFileIO>       m_sceneManagerFileIO    = nullptr;
		std::unique_ptr<MRI::Converter::SceneManagerConverter> m_sceneManagerConverter = nullptr;

		std::uint32_t m_nextSceneTag = MRI::CommonConstant::k_invalidStaticID;

		//===============================
		// シングルトン
		//===============================
		friend class MRI::SingletonBase<MRI::SceneManager>;
	
		SceneManager ();
		~SceneManager() override;
	};
}