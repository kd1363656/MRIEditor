#pragma once

namespace MRI::Converter
{
	class SceneManagerConverter final : public MRI::Converter::IConverter
	{
	public:

		SceneManagerConverter ()          = default;
		~SceneManagerConverter() override = default;

		void		   Deserialize(const nlohmann::json& a_json) override;
		nlohmann::json Serialize  ()							 override;
	};
}