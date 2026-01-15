#pragma once

namespace MRI::Helper
{
	class AssetFilePathHelper final
	{
	public:

		AssetFilePathHelper () = default;
		~AssetFilePathHelper() = default;

		void Init();

		void EditPrefabInspector();

		void DeserializePrefab(const nlohmann::json& a_json);

		nlohmann::json SerializePrefab();

		void SetFilters(const char* a_set) { m_filters = a_set; }

		const std::string& GetFilePath() const { return m_filePath; }

		bool GetIsChanged() const { return m_isChanged; }

	private:

		std::string m_filePath = std::string();
		const char* m_filters  = "";

		bool m_isChanged = false;
	};
}