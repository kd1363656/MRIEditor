#pragma once

namespace MRI
{
	class GameObject final : public std::enable_shared_from_this<MRI::GameObject>
	{
	public:

		GameObject ();
		~GameObject();

		void Init        ();
		void PostLoadInit() const;

		void PreUpdate  ();
		void EarlyUpdate();
		void Update     () const;
		void LateUpdate () const;
		void FixMatrix  ();

		void Destroy();

		void EditPrefabInspector();
		void EditSpawnInspector () const;

		void Deserialize      (const nlohmann::json& a_json)																																			 const;
		void DeserializeSpawn (const nlohmann::json& a_json , std::vector<std::shared_ptr<MRI::Component::ComponentBase>>& a_componentList , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const;
		void DeserializePrefab(const nlohmann::json& a_json , std::vector<std::shared_ptr<MRI::Component::ComponentBase>>& a_componentList , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const;

		nlohmann::json SerializeSpawn () const;
		nlohmann::json SerializePrefab() const;

		void AddComponent(const std::shared_ptr<MRI::Component::ComponentBase> a_component);

		void CreateParentChildRelationship(std::weak_ptr<MRI::GameObject>       a_childCache);
		void RemoveChildCache             (const std::weak_ptr<MRI::GameObject> a_childCache);

		void ResetParentCache() { m_parentCache.reset(); }

		bool HasParent() const { return !m_parentCache.expired(); }

		std::weak_ptr<MRI::Observer::Observer>            GetComponentObserverCache     () { return m_componentObserver;  }
		std::weak_ptr<MRI::Component::TransformComponent> GetSelfTransformComponentCache() { return m_transformComponent; }

		template <MRI::Concept::IsDerivedComponentBaseConcept Type>
		std::weak_ptr<Type> GetComponentCache() const
		{
			const std::uint32_t l_id = MRI::StaticID::GetTypeID<Type>();
		
			if (auto l_itr = m_uniqueComponentCacheMap.find(l_id); 
				l_itr != m_uniqueComponentCacheMap.end())
			{
				if (auto l_component = l_itr->second.lock())
				{
					return std::static_pointer_cast<Type>(l_component);
				}
			}
	
			return std::weak_ptr<Type>();
		}
	
		template <MRI::Concept::IsDerivedComponentBaseConcept Type>
		std::vector<std::weak_ptr<Type>> GetMultiComponentCacheList() const
		{
			const std::uint32_t l_id = MRI::StaticID::GetTypeID<Type>();
			std::vector<std::shared_ptr<Type>> l_list = {};
	
			if (auto l_itr = m_multiComponentCacheMap.find(l_id); 
				l_itr != m_multiComponentCacheMap.end())
			{
				l_list.emplace_back(std::static_pointer_cast<Type>(l_itr->second));
			}
	
			return l_list;
		}

		const auto& GetChildCacheList() const { return m_childCacheList; }

		const auto& GetComponentList    () const { return m_componentList; }
		auto&       GetWorkComponentList()		 { return m_componentList; }

		std::weak_ptr<MRI::GameObject> GetParentCache() const { return m_parentCache; }

		std::string_view GetPrefabName() const { return m_prefabName; }

		const UUID& GetUUID() const { return m_uuid; }

		bool GetIsDestroyed() const { return m_isDestroyed; }

		void SetParentCache(const std::weak_ptr<MRI::GameObject> a_parentCache);

		void SetPrefabName(const std::string_view a_set) { m_prefabName = a_set; }

		void SetUUID(const UUID& a_set) { m_uuid = a_set; }

	private:

		std::list<std::weak_ptr<MRI::GameObject>> m_childCacheList;

		std::unordered_map<std::uint32_t, std::weak_ptr<MRI::Component::ComponentBase>>				 m_uniqueComponentCacheMap;
		std::unordered_map<std::uint32_t, std::vector<std::weak_ptr<MRI::Component::ComponentBase>>> m_multiComponentCacheMap;

		std::vector<std::shared_ptr<MRI::Component::ComponentBase>> m_componentList;

		std::shared_ptr<MRI::Observer::Observer>            m_componentObserver  = nullptr;
		std::shared_ptr<MRI::Component::TransformComponent> m_transformComponent = nullptr;

		std::weak_ptr<MRI::GameObject> m_parentCache;

		std::unique_ptr<MRI::Converter::GameObjectConverter> m_gameObjectConverter = nullptr;

		UUID m_uuid = GUID_NULL;

		std::string m_prefabName = std::string();

		bool m_isDestroyed = false;
	};
}