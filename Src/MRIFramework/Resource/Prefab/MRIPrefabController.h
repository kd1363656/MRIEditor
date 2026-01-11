#pragma once

namespace MRI
{
	namespace Converter
	{
		class PrefabControllerConverter;
	}

	namespace FileIO
	{
		class PrefabControllerFileIO;
	}

	class GameObject;
}

namespace MRI::Resource
{
	class PrefabController final : public std::enable_shared_from_this<MRI::Resource::PrefabController>
	{
	public:

		PrefabController () = default;
		~PrefabController() = default;

		void Init();

		void LoadFile();
		void SaveFile();

		void           Deserialize(const nlohmann::json& a_json);
		nlohmann::json Serialize  ()							 const;

		bool AddPrefab(const std::string& a_prefabName , const std::shared_ptr<MRI::Resource::Prefab>& a_prefab)
		{
			const bool l_isSuccess = m_prefabMap.try_emplace(a_prefabName , a_prefab).second;

			if (!l_isSuccess)
			{
				MRI_ADD_LOG("プレハブ%sはすでに登録されています" , a_prefabName.c_str());
			}

			return l_isSuccess;
		}

		void RemovePrefab(const std::string& a_prefabKey);

		std::weak_ptr<MRI::Resource::Prefab> FetchPrefabCache(const std::string_view& a_prefabName);

		const auto& GetPrefabMap    () const { return m_prefabMap; }
		auto&		GetWorkPrefabMap()		 { return m_prefabMap; }

	private:

		std::unique_ptr<MRI::Converter::PrefabControllerConverter> m_prefabControllerConverter = nullptr;
		std::unique_ptr<MRI::FileIO::PrefabControllerFileIO>       m_prefabControllerFileIO    = nullptr;

		std::unordered_map<std::string , std::shared_ptr<MRI::Resource::Prefab> , MRI::CommonStruct::StringHash , std::equal_to<>> m_prefabMap;
	};
}