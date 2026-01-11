#pragma once

namespace MRI::Converter
{
	class PrefabControllerConverter final : public MRI::Converter::IConverter
	{
	public:
		PrefabControllerConverter ()          = default;
		~PrefabControllerConverter() override = default;

		void		   Deserialize(const nlohmann::json& a_json) override;
		nlohmann::json Serialize  ()							 override;

		void SetPrefabControllerCache(const std::weak_ptr<MRI::Resource::PrefabController>& a_set) { m_prefabControllerCache = a_set; }

	private:

		std::weak_ptr<MRI::Resource::PrefabController> m_prefabControllerCache;
	};
}