#pragma once

namespace MRI::CommonConstant
{

	static constexpr std::string_view k_jsonExtension = ".json";

	static constexpr std::size_t k_vkCodeMaxNum = static_cast<std::size_t>(VK_OEM_CLEAR + 1);

	std::uint32_t k_invalidStaticID = 0u;

	static constexpr int k_defaultWindowHeight = 1280;
	static constexpr int k_defaultWindowWidth  = 720;
}