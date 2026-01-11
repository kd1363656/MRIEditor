#include "MRISceneManagerFileIO.h"

void MRI::FileIO::SceneManagerFileIO::LoadFile()
{
	auto l_rootJson = MRI::FileIOUtility::LoadJsonFile(GetFileIOPath());
	if (l_rootJson.is_null()) { return; }

	// シーンマネージャーの情報を読み込む
	MRI::SceneManager::GetInstance().Deserialize(l_rootJson);
}
void MRI::FileIO::SceneManagerFileIO::SaveFile()
{
	auto l_rootJson = nlohmann::json();

	// シーンマネージャーの情報を保存
	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::SceneManager::GetInstance().Serialize());
	
	// 書き込めておらず空の"json"なら"return"
	if (l_rootJson.is_null()) { return; }

	// 保存した情報を今のアセットのファイルパスにある"json"ファイルに書き込む
	MRI::FileIOUtility::SaveJsonFile(l_rootJson , GetFileIOPath());
}