#pragma once

namespace MRI::VKCodeUtility
{
	static constexpr int k_keyNameBufferSize = 128;
	static constexpr int k_scanCodeShiftBits = 16;

	// キーコードから名前を取得
	inline std::string GetKeyName(const int a_vkCode)
	{
		// 仮想キーコードの最大数よりも大きい値の場合"return"
		if (a_vkCode >= MRI::CommonConstant::k_vkCodeMaxNum) 
		{
			return std::string();
		}

		// 仮想キーコードをスキャンコードに変換
		// "WindowsAPI"の"GetKeyNameTextW"が受け取る"lParam"に合わせるため左に"16"ビットシフト
		const int l_scanCode = MapVirtualKey(a_vkCode , MAPVK_VK_TO_VSC) << k_scanCodeShiftBits;
		
		// "ScanCode"をもとに対応するキー名を取得
		// "Unicode"対応の"GetKeyNameTextW"を使用
		if (WCHAR l_keyName[k_keyNameBufferSize];
			GetKeyNameTextW(l_scanCode , l_keyName , sizeof(k_keyNameBufferSize)) > 0)
		{
			// "UTF-16 -> UTF - 8"変換
			std::wstring_convert<std::codecvt_utf8<wchar_t>> l_conv;
			return l_conv.to_bytes(l_keyName);
		}

		return std::string();
	}
}