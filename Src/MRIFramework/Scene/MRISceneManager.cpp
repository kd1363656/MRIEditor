#include "MRISceneManager.h"

void MRI::SceneManager::Init()
{
	if (!m_scene)
	{
		m_scene = std::make_shared<MRI::Scene>();
	}

	m_scene->Init();

	if (!m_playerKeyConfig)
	{
		m_playerKeyConfig = std::make_shared<MRI::PlayerKeyConfig>();
	}

	m_playerKeyConfig->Init();

	if (!m_sceneManagerFileIO) 
	{
		m_sceneManagerFileIO = std::make_unique<MRI::FileIO::SceneManagerFileIO>();
	}

	// 初期化してから読み込むファイルのパスを格納
	m_sceneManagerFileIO->Init();
	
	if (!m_sceneManagerConverter)
	{
		m_sceneManagerConverter = std::make_unique<MRI::Converter::SceneManagerConverter>();
	}
}
void MRI::SceneManager::PostLoadInit() const
{
	if (!m_scene) { return; }
	m_scene->PostLoadInit();
}

void MRI::SceneManager::LoadFile()
{
	if (!m_sceneManagerFileIO || !m_playerKeyConfig) { return; }
	
	m_playerKeyConfig->LoadFile   ();
	m_sceneManagerFileIO->LoadFile();
}
void MRI::SceneManager::SaveFile() const
{
	if (!m_sceneManagerFileIO || !m_playerKeyConfig) { return; }
	
	m_playerKeyConfig->SaveFile   ();
	m_sceneManagerFileIO->SaveFile();
}

void MRI::SceneManager::SceneShift()
{
	// もしタグがセットされたそれに対応するファイルパスを取得できた場合
	// シーン遷移を行いファイルを読み込む
	if (!m_sceneManagerFileIO || !m_scene)						  { return; }
	if (m_nextSceneTag == MRI::CommonConstant::k_invalidStaticID) { return; }

	const std::string l_filePath = FetchSceneShiftLoadFilePath(m_nextSceneTag);
	SceneShift(l_filePath);
}
void MRI::SceneManager::SceneShift(const std::string& a_filePath)
{
	// ファイルパスが空なら"return"
	if (a_filePath.empty()) { return; }

	// 自身のシーン遷移で反応するタグをクリア
	ClearSceneShiftMap();

	// シーンの初期化
	m_scene->Init();

	// シーンをロードするファイルパスを変更
	m_sceneManagerFileIO->SetFileIOPath(a_filePath);
	m_sceneManagerFileIO->LoadFile     ();

	// ファイルの読み込みが終わったらタグを初期化
	m_nextSceneTag = MRI::CommonConstant::k_invalidStaticID;
}

void MRI::SceneManager::PreUpdate()
{
	SceneShift();

	if (!m_scene) { return; }
	m_scene->PreUpdate();
}
void MRI::SceneManager::EarlyUpdate()
{
	if (!m_scene) { return; }
	m_scene->EarlyUpdate();
}
void MRI::SceneManager::Update() const
{
	if (!m_scene) { return; }
	m_scene->Update();
}
void MRI::SceneManager::LateUpdate() const
{
	if (!m_scene) { return; }
	m_scene->LateUpdate();
}
void MRI::SceneManager::FixMatrix()
{
	if (!m_scene) { return; }
	m_scene->FixMatrix();
}

void MRI::SceneManager::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null())					  { return; }
	if (!m_scene || !m_sceneManagerConverter) { return; }
	
	// シーンマネージャーの情報を保存
	m_sceneManagerConverter->Deserialize(a_json);

	// シーンの情報を読み込む
	m_scene->Deserialize(a_json);
}
nlohmann::json MRI::SceneManager::Serialize()
{
	auto l_rootJson  = nlohmann::json();
	
	if (!m_scene || !m_sceneManagerConverter)
	{
		return l_rootJson; 
	}

	// シーンマネージャーの情報を保存
	MRI::JsonUtility::UpdateJson(l_rootJson , m_sceneManagerConverter->Serialize());

	// シーンの情報を保存
	MRI::JsonUtility::UpdateJson(l_rootJson , m_scene->Serialize());

	return l_rootJson;
}

void MRI::SceneManager::RegisterSceneShiftMap(const std::uint32_t a_tag, const std::string& a_sceneFilePath)
{
	m_sceneShiftMap.try_emplace(a_tag , a_sceneFilePath);
}
void MRI::SceneManager::ClearSceneShiftMap()
{
	m_sceneShiftMap.clear();
}

std::string MRI::SceneManager::FetchSceneShiftLoadFilePath(const std::uint32_t a_tag)
{
	auto l_itr = m_sceneShiftMap.find(a_tag);
	if (l_itr == m_sceneShiftMap.end()) 
	{
		return std::string();
	}

	return l_itr->second;
}

void MRI::SceneManager::SetSceneManagerFileIOPath(const std::string& a_set)
{
	if (!m_sceneManagerFileIO) { return; }
	m_sceneManagerFileIO->SetFileIOPath(a_set);
}