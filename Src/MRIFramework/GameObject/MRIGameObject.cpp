#include "MRIGameObject.h"

MRI::GameObject::GameObject () = default;
MRI::GameObject::~GameObject() = default;

void MRI::GameObject::Init()
{
	for (const auto& l_component : m_componentList)
	{
		if (!l_component) { continue; }
		l_component->Init();
	}

	if (!m_componentObserver)
	{
		m_componentObserver = std::make_shared<MRI::Observer::Observer>();
	}

	m_componentObserver->Init();

	if (!m_transformComponent)
	{
		m_transformComponent = std::make_shared<MRI::Component::TransformComponent>();
	}

	m_transformComponent->Init         ();
	m_transformComponent->SetOwnerCache(weak_from_this());

	if (!m_gameObjectConverter)
	{
		m_gameObjectConverter = std::make_unique<MRI::Converter::GameObjectConverter>();
	}

	m_gameObjectConverter->SetOwnerCache(weak_from_this());

	m_uuid = GUID_NULL;

	m_prefabName = std::string();

	m_isDestroyed = false;
}
void MRI::GameObject::PostLoadInit() const
{
	for (const auto& l_component : m_componentList)
	{
		if (!l_component) { continue; }
		l_component->PostLoadInit();
	}

	// ロードした座標や回転率から行列の確定を行う
	if (m_transformComponent)
	{
		m_transformComponent->PostLoadInit();
	}
}

void MRI::GameObject::PreUpdate()
{
	// 存在しない子のキャッシュを削除
	std::erase_if(m_childCacheList , [](const auto& a_pair) -> bool
	{
		return a_pair.expired();
	});

	// 通知の初期化("Moment"通知のみ)
	if (m_componentObserver)
	{
		m_componentObserver->BeginFrame();
	}

	for (const auto& l_component : m_componentList)
	{
		if (!l_component) { continue; }
		l_component->PreUpdate();
	}
}
void MRI::GameObject::EarlyUpdate()
{
	for (const auto& l_component : m_componentList)
	{
		if (!l_component) { continue; }
		l_component->EarlyUpdate();
	}
}
void MRI::GameObject::Update() const
{
	for (const auto& l_component : m_componentList)
	{
		if (!l_component) { continue; }
		l_component->Update();
	}
}
void MRI::GameObject::LateUpdate() const
{
	for (const auto& l_component : m_componentList)
	{
		if (!l_component) { continue; }
		l_component->LateUpdate();
	}
}

void MRI::GameObject::FixMatrix()
{
	if (!m_transformComponent) { return; }
	m_transformComponent->FixMatrix();
}

void MRI::GameObject::Destroy()
{
	m_isDestroyed = true;

	// 子も削除を実行
	for (const auto& l_childCache : m_childCacheList)
	{
		auto l_child = l_childCache.lock();
		if (!l_child) { continue; }

		l_child->Destroy();
	}
}

void MRI::GameObject::EditPrefabInspector()
{
	for (const auto& l_component : m_componentList)
	{
		if (!l_component) { continue; }
		l_component->EditPrefabInspector();
	}

	if (m_componentObserver)
	{
		m_componentObserver->EditPrefabInspector<MRI::Tag::ComponentEventTagBase>();
	}
}
void MRI::GameObject::EditSpawnInspector() const
{
	for (const auto& l_component : m_componentList)
	{
		if (!l_component) { continue; }
		l_component->EditSpawnInspector();
	}
}

void MRI::GameObject::Deserialize(const nlohmann::json& a_json) const
{
	if (a_json.is_null())       { return; }
	if (!m_gameObjectConverter) { return; }

	m_gameObjectConverter->Deserialize(a_json);
}
void MRI::GameObject::DeserializeSpawn(const nlohmann::json& a_json , std::vector<std::shared_ptr<MRI::Component::ComponentBase>>& a_componentList , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const
{
	if (a_json.is_null())		{ return; }
	if (!m_gameObjectConverter) { return; }

	m_gameObjectConverter->DeserializeSpawn(a_json , a_componentList , a_childLoadList);
}
void MRI::GameObject::DeserializePrefab(const nlohmann::json& a_json , std::vector<std::shared_ptr<MRI::Component::ComponentBase>>& a_componentList , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const
{
	if (a_json.is_null())		{ return; }
	if (!m_gameObjectConverter) { return; }

	m_gameObjectConverter->DeserializePrefab(a_json , a_componentList , a_childLoadList);
}

nlohmann::json MRI::GameObject::SerializeSpawn() const
{
	if (!m_gameObjectConverter)
	{
		return nlohmann::json(); 
	}

	auto l_rootJson       = nlohmann::json						 ();
	auto l_gameObjectJson = m_gameObjectConverter->SerializeSpawn();

	MRI::JsonUtility::UpdateJson(l_rootJson , l_gameObjectJson);
	return l_rootJson;
}
nlohmann::json MRI::GameObject::SerializePrefab() const
{
	if (!m_gameObjectConverter)
	{
		return nlohmann::json(); 
	}

	auto l_rootJson       = nlohmann::json						  ();
	auto l_gameObjectJson = m_gameObjectConverter->SerializePrefab();

	MRI::JsonUtility::UpdateJson(l_rootJson , l_gameObjectJson);
	return l_rootJson;
}

void MRI::GameObject::AddComponent(const std::shared_ptr<MRI::Component::ComponentBase> a_component)
{
	if (!a_component) { return; }

	const std::uint32_t l_id = a_component->GetTypeInfo().k_id;

	bool l_canAdd = false;

	// 複数生成を許可されているかそうじゃないかで格納するキャッシュマップを変える
	if (!a_component->IsAllowMultiple())
	{
		// もしすでにコンポーネントが追加されていれば追加できなかったという結果が帰って来るので
		// コンポーネントリスト本体に追加できるかどうかのフラグに結果を格納
		l_canAdd = m_uniqueComponentCacheMap.try_emplace(l_id , a_component).second;
	}
	else
	{
		m_multiComponentCacheMap[l_id].emplace_back(a_component);
		// 重複格納を許可
		l_canAdd = true;
	}

	// コンポーネント本体を管理するコンポーネントリストに格納
	if (l_canAdd)
	{
		m_componentList.emplace_back(a_component);
	}
}

void MRI::GameObject::CreateParentChildRelationship(std::weak_ptr<MRI::GameObject> a_childCache)
{
	auto l_child = a_childCache.lock();
	if (!l_child) { return; }

	// 自分自身を子にしない
	if (l_child.get() == this) { return; }

	// すでに同じ子がいる子をチェック、もし同じ子なら"return"
	auto l_itr = std::find_if(m_childCacheList.begin(), m_childCacheList.end(), [l_child](const std::weak_ptr<MRI::GameObject> a_inListChildCache)
		{
			auto l_inListChild = a_inListChildCache.lock();
			if (!l_inListChild)
			{
				return false;
			}

			// ポインタ同士を比較
			return l_inListChild.get() == l_child.get();
		});

	// もし同じポインタなら"return"
	if (const bool l_isInListChild = l_itr != m_childCacheList.end();
		l_isInListChild)
	{
		return;
	}

	// 追加する予定の子の親をリセット
	l_child->ResetParentCache();

	// 親子関係を構築
	l_child->SetParentCache(weak_from_this());
	m_childCacheList.emplace_back(l_child);

	auto l_childTransformComponentCache = l_child->GetSelfTransformComponentCache().lock();
	if (!l_childTransformComponentCache) { return; }

	// 子のTransformComponentに、親のTransformComponentをキャッシュする
	l_childTransformComponentCache->SetParentTransformComponentCache(GetSelfTransformComponentCache());
}
void MRI::GameObject::RemoveChildCache(const std::weak_ptr<MRI::GameObject> a_childCache)
{
	auto l_child = a_childCache.lock();
	if (!l_child) { return; }

	m_childCacheList.remove_if([l_child](const std::weak_ptr<MRI::GameObject> a_inListChildCache)
	{
		if (auto l_inListChild = a_inListChildCache.lock())
		{
			// ポインタを比較
			return l_child.get() == l_inListChild.get();
		}

		return false;
	});
}

void MRI::GameObject::ResetParentCache()
{
	m_parentCache.reset();

	auto l_selfTransformComponentCache = GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	// GameObject側の親子関係を解除したので、
	// TransformComponent側の親Transformキャッシュも解除する
	l_selfTransformComponentCache->SetParentTransformComponentCache(std::weak_ptr<MRI::Component::TransformComponent>());
}

void MRI::GameObject::SetParentCache(const std::weak_ptr<MRI::GameObject> a_parentCache)
{
	// なにも格納されていなければキャッシュに登録しない
	if (a_parentCache.expired()) { return; }
	m_parentCache = a_parentCache;
}