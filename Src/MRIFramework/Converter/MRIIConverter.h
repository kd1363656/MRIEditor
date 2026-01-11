#pragma once

namespace MRI::Converter
{
	class IConverter
	{
	public:

		IConverter         () = default;
		virtual ~IConverter() = default;

		virtual void Init() { /*必要に応じてオーバーライドしてください*/ };

		virtual void           Deserialize(const nlohmann::json& a_json) = 0;
		virtual nlohmann::json Serialize  ()							 = 0;
	};
}