#include "MRIScene.h"

MRI::Scene::Scene () = default;
MRI::Scene::~Scene() = default;

void MRI::Scene::Init()
{
	if (!m_sceneShiftObserver)
	{
		m_sceneShiftObserver = std::make_shared<MRI::Observer::Observer>();
	}

	m_sceneShiftObserver->Init();

	if (!m_resourceController)
	{
		m_resourceController = std::make_shared<MRI::Resource::ResourceController>();
	}

	m_resourceController->Init();

	if (!m_sceneConverter)
	{
		m_sceneConverter = std::make_unique<MRI::Converter::SceneConverter>();
	}

	// シーンとプレハブコントローラーのポインタを持たせる
	m_sceneConverter->SetSceneCache           (weak_from_this								 ());
	m_sceneConverter->SetPrefabControllerCache(m_resourceController->GetPrefabControllerCache());

	m_gameObjectList.clear();

	MRI::RenderManager::GetInstance().Init();
}
void MRI::Scene::PostLoadInit() const
{
	for (const auto& l_gameObject : m_gameObjectList)
	{
		if (!l_gameObject) { continue; }
		l_gameObject->PostLoadInit();
	}

	MRI::RenderManager::GetInstance().PostLoadInit();
}

void MRI::Scene::PreUpdate()
{
	// 削除フラグが立っているゲームオブジェクトを削除
	std::erase_if(m_gameObjectList , [](const auto& a_pair)
	{
		return a_pair->GetIsDestroyed();
	});

	for (const auto& l_gameObject : m_gameObjectList)
	{
		if (!l_gameObject) { continue; }
		l_gameObject->PreUpdate();
	}
}
void MRI::Scene::EarlyUpdate() const
{
	for (const auto& l_gameObject : m_gameObjectList)
	{
		if (!l_gameObject) { continue; }
		l_gameObject->EarlyUpdate();
	}
}
void MRI::Scene::Update() const
{
	for (const auto& l_gameObject : m_gameObjectList)
	{
		if (!l_gameObject) { continue; }
		l_gameObject->Update();
	}
}
void MRI::Scene::LateUpdate() const
{
	for (const auto& l_gameObject : m_gameObjectList)
	{
		if (!l_gameObject) { continue; }
		l_gameObject->LateUpdate();
	}
}
void MRI::Scene::FixMatrix() const
{
	for (const auto& l_gameObject : m_gameObjectList)
	{
		if (!l_gameObject) { continue; }
		l_gameObject->FixMatrix();
	}
}

void MRI::Scene::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null())													 { return; }
	if (!m_resourceController || !m_sceneShiftObserver || !m_sceneConverter) { return; }
	
	// リソースのデシリアライズ
	m_resourceController->Deserialize(a_json);

	// デシリアライズした結果をもとにファイルをロード
	m_resourceController->LoadFile();

	// シーンのデシリアライズ
	m_sceneConverter->Deserialize    (a_json);
	m_sceneShiftObserver->Deserialize(a_json);
}
nlohmann::json MRI::Scene::Serialize() const
{
	auto l_rootJson = nlohmann::json();

	if (!m_resourceController || !m_sceneShiftObserver || !m_sceneConverter)
	{
		return l_rootJson;
	}

	// リソースのファイルパスをセーブ
	m_resourceController->SaveFile();

	MRI::JsonUtility::UpdateJson(l_rootJson , m_resourceController->Serialize());
	MRI::JsonUtility::UpdateJson(l_rootJson , m_sceneConverter->Serialize    ());
	MRI::JsonUtility::UpdateJson(l_rootJson , m_sceneShiftObserver->Serialize());

	return l_rootJson;
}

void MRI::Scene::AddGameObject(const std::shared_ptr<MRI::GameObject> a_gameObject)
{
	if (!a_gameObject) { return; }

	// すでに同じ子がいるかをチェック、もし同じ子なら"return"
	if(auto l_itr = std::find(m_gameObjectList.begin() , m_gameObjectList.end() , a_gameObject);
	   l_itr != m_gameObjectList.end())
	{
		return;
	}

	m_gameObjectList.emplace_back(a_gameObject);
}

std::weak_ptr<MRI::Resource::PrefabController> MRI::Scene::GetPrefabControllerCache() const
{
	if (!m_resourceController)
	{
		return std::weak_ptr<MRI::Resource::PrefabController>();
	}

	// プレハブコントローラーの実態があるかどうかを確認
	if (m_resourceController->GetPrefabControllerCache().expired())
	{
		return std::weak_ptr<MRI::Resource::PrefabController>();
	}

	return m_resourceController->GetPrefabControllerCache();
}