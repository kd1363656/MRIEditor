#pragma once

namespace MRI
{
	class GameObject;
}

namespace MRI::Resource
{
	class Prefab final
	{
	public:

		Prefab () = default;
		~Prefab() = default;

		void LoadPrefab();
		void SavePrefab() const;

		void           Deserialize(const nlohmann::json& a_json);
		nlohmann::json Serialize  ()							 const;

		std::weak_ptr<MRI::GameObject> GetGameObjectCache() const { return m_gameObjectCache; }

		std::string_view GetFilePath() const { return m_filePath; }

		const nlohmann::json& GetJson() const { return m_json; }

		void SetGameObjectCache(const std::weak_ptr<MRI::GameObject> a_set) { m_gameObjectCache = a_set; }

		void SetFilePath(const std::string& a_set) { m_filePath = a_set; }

	private:

		std::weak_ptr<MRI::GameObject> m_gameObjectCache;

		std::string m_filePath = std::string();

		nlohmann::json m_json = nlohmann::json();
	};
}