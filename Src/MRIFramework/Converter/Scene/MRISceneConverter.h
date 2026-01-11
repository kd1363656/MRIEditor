#pragma once

namespace MRI
{
	class Scene;
}

namespace MRI::Converter
{
	class SceneConverter final : public MRI::Converter::IConverter
	{
	public:

		SceneConverter ()		   = default;
		~SceneConverter() override = default;

		void		   Deserialize(const nlohmann::json& a_json) override;
		nlohmann::json Serialize  ()							 override;

		void SetSceneCache           (const std::weak_ptr<MRI::Scene>&					    a_set) { m_sceneCache            = a_set; }
		void SetPrefabControllerCache(const std::weak_ptr<MRI::Resource::PrefabController>& a_set) { m_prefabControllerCache = a_set; }

	private:

		void DeserializeGameObject (const nlohmann::json& a_json) const;
		void DecidePrefabGameObject()							  const;

		std::weak_ptr<MRI::Scene>					   m_sceneCache;
		std::weak_ptr<MRI::Resource::PrefabController> m_prefabControllerCache;
	};
}