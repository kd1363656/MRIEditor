#pragma once

namespace MRI::CommonConstant
{
	static constexpr std::string_view k_stringUnknown = "Unknown";

	static constexpr std::string_view k_jsonExtension = ".json";
	
	static constexpr Math::Color k_colorLess = { 0.0F , 0.0F , 0.0F , 1.0F };

	static constexpr float k_epsilon = 0.00001F;

	static constexpr float k_interpolatorProgressMax = 1.0F;
	static constexpr float k_quarterDegree           = 90.0F;

	static constexpr std::uint32_t k_invalidStaticID = 0U;

	static constexpr int k_defaultWindowHeight = 1280;
	static constexpr int k_defaultWindowWidth  = 720;

	// 本来なら"C++"なら"std::size_t"として扱いたいが仮想キーコードを扱う際に使う値のため"int"型で定義
	static constexpr int k_vkCodeMaxNum = static_cast<std::size_t>(VK_OEM_CLEAR + 1);
}