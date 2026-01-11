#pragma once

namespace MRI::Resource
{
	// フライウェイトで管理しているリソースの開放、プレハブ、音の管理といったリソースを一括で管理するクラス
	class ResourceController final
	{
	public:

		ResourceController () = default;
		~ResourceController() = default;

		void Init();

		void LoadFile();
		void SaveFile();

		void           Deserialize(const nlohmann::json& a_json);
		nlohmann::json Serialize  ()							 const;

		void ClearAsset() const;

		std::weak_ptr<MRI::Resource::PrefabController> GetPrefabControllerCache() const { return m_prefabController; }

	private:

		std::shared_ptr<MRI::Resource::PrefabController> m_prefabController = nullptr;
	};
}