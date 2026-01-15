#include "MRIAssetFilePathHelper.h"

void MRI::Helper::AssetFilePathHelper::Init()
{
	m_filePath.clear();
	m_filters = "";

	m_isChanged = false;
}

void MRI::Helper::AssetFilePathHelper::EditPrefabInspector()
{
	// 毎フレーム変更検知フラグをリセットすることで
	// 一フレームだけ変更したかどうかのフラグを取得することが可能
	m_isChanged = false;

	// ファイルパスが存在しなければ"Empty"をそうでなければファイルパスを表示
	const char* const l_filePath = m_filePath.empty() ? "Empty" : m_filePath.c_str();
	ImGui::Text("AssetFilePath : %s" , l_filePath);

	if (!MRI::EditorUtility::SameLineSmallButton("ChangeFilePath"))													 { return; }
	if (!MRI::EditorUtility::OpenFileDialog(m_filePath , MRI::EditorCommonConstant::k_defaultTitleOpen , m_filters)) { return; }

	m_isChanged = true;
}

void MRI::Helper::AssetFilePathHelper::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	m_filePath = a_json.value("FilePath" , m_filePath);
}

nlohmann::json MRI::Helper::AssetFilePathHelper::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	l_rootJson["FilePath"] = m_filePath;

	return l_rootJson;
}