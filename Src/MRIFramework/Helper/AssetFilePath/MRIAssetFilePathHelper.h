#pragma once

namespace MRI::Helper
{
	class AssetFilePathHelper
	{
	public:

		AssetFilePathHelper () = default;
		~AssetFilePathHelper() = default;

		void Init();

		void EditPrefabInspector();

		void DeserializePrefab(const nlohmann::json& a_json);

		nlohmann::json SerializePrefab();

		void SetFilters(const std::string& a_set) { m_filters = a_set; }

		const std::string& GetFilePath() const { return m_filePath; }

	private:

		std::string m_filePath = std::string();
		std::string m_filters  = std::string();

		bool m_isChanged = false;
	};
}