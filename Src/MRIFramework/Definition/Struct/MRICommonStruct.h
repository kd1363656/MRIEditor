#pragma once

namespace MRI::CommonStruct
{
	struct StringHash final
	{
		// "C++14"以降で追加された"TransparentLookUp"を生かした設計手法
		using is_transparent = void;

		// "std::hash<std::string>{}"は一時インスタンスを作製してその"()"に"Key"を格納しているという構文
		std::size_t operator()(const std::string& a_key) const { return std::hash<std::string>     {}(a_key); }
		std::size_t operator()(std::string_view   a_key) const { return std::hash<std::string_view>{}(a_key); }
		std::size_t operator()(const char*        a_key) const { return std::hash<std::string_view>{}(a_key); }
	};

	struct Size final
	{
		int height = 0;
		int width  = 0;
	};
}