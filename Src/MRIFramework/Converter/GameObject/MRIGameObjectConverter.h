#pragma once

namespace MRI
{
	class GameObject;
}

namespace MRI::Converter
{
	class GameObjectConverter final
	{
	public:

		GameObjectConverter () = default;
		~GameObjectConverter() = default;

		void Deserialize(const nlohmann::json& a_json) const;

		void DeserializePrefab(const nlohmann::json& a_json , std::vector<std::shared_ptr<MRI::Component::ComponentBase>>& a_componentList , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const;
		void DeserializeSpawn (const nlohmann::json& a_json , std::vector<std::shared_ptr<MRI::Component::ComponentBase>>& a_componentList , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const;

		nlohmann::json SerializePrefab() const;
		nlohmann::json SerializeSpawn () const;

		void SetOwnerCache(const std::weak_ptr<MRI::GameObject>& a_set) { m_ownerCache = a_set; }

	private:

		void DeserializeComponentObserver(const nlohmann::json& a_json)																 const;
		void DeserializeChildPrefab      (const nlohmann::json& a_json , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const;
		void DeserializeChildSpawn       (const nlohmann::json& a_json , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const;

		nlohmann::json SerializeComponentObserver() const;
		nlohmann::json SerializeChildPrefab      () const;
		nlohmann::json SerializeChildSpawn       () const;

		std::weak_ptr<MRI::GameObject> m_ownerCache;
	};
}