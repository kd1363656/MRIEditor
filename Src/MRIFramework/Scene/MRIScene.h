#pragma once

namespace MRI
{
	namespace Converter
	{
		class SceneConverter;
	}

	namespace Resource
	{
		class ResourceController;
		class PrefabController;
	}
}

namespace MRI
{
	class Scene final : public std::enable_shared_from_this<MRI::Scene>
	{
	public:

		Scene ();
		~Scene();

		void Init        ();
		void PostLoadInit() const;

		void PreUpdate  ();
		void EarlyUpdate() const;
		void Update     () const;
		void LateUpdate () const;
		void FixMatrix  () const;

		void           Deserialize(const nlohmann::json& a_json);
		nlohmann::json Serialize  ()					         const;
	
		void AddGameObject(const std::shared_ptr<MRI::GameObject> a_gameObject);

		std::weak_ptr<MRI::Observer::Observer>		   GetSceneShiftObserverCache() const { return m_sceneShiftObserver; }
		std::weak_ptr<MRI::Resource::PrefabController> GetPrefabControllerCache  () const;

		const auto& GetGameObjectList() const { return m_gameObjectList; }

		auto& GetWorkGameObjectList() { return m_gameObjectList; }

	private:

		std::list<std::shared_ptr<MRI::GameObject>> m_gameObjectList;
		
		std::shared_ptr<MRI::Observer::Observer>           m_sceneShiftObserver = nullptr;
		std::shared_ptr<MRI::Resource::ResourceController> m_resourceController = nullptr;

		std::unique_ptr<MRI::Converter::SceneConverter> m_sceneConverter = nullptr;
	};
}