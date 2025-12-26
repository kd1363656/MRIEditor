#pragma once

namespace MRI
{
	class InputManager final : public MRI::SingletonBase<MRI::InputManager>
	{
	private:

		enum class DeviceIndex : std::size_t
		{
			KeyBoard    ,
			Mouse       ,
			DeviceCount ,
		};

	public:

		void Init          ();
		void RegisterDevice();

		void ResetInputStates();
	
		bool IsInputHeld(const int a_vkCode) const;
		bool IsInputOnce(const int a_vkCode) const;
		
		void ProcessInput(const LPARAM a_lParam);
	
		void BackUpInputState();

		const auto& GetNowInputStateList() const { return m_nowInputStateList; }
		const auto& GetOldInputStateList() const { return m_oldInputStateList; }
	
		MRI::CommonStruct::Mouse GetNowMouse() const { return m_nowMouse; }

		void SetHWND(const HWND a_set) { m_hWND = a_set; }

	private:
	
		void Register(RAWINPUTDEVICE& a_rid       , 
					  const USHORT    a_usagePage , 
					  const USHORT	  a_usage     , 
					  const DWORD     a_dwFlags);
		
		void UpdateMouseButtonState(const WORD a_flags    , 
									const WORD a_downFlag , 
									const WORD a_upFlag   , 
									const int  a_vkCode);

		static constexpr std::size_t k_rawInputBufferSize = 1024ULL;

		static constexpr DWORD k_inputFlags = RIDEV_INPUTSINK;

		static constexpr USHORT k_usagePageGeneric = 0x01;
		static constexpr USHORT k_usageKeyBoard    = 0x06;
		static constexpr USHORT k_usageMouse       = 0x02;

		// キーの入力状態を毎フレーム格納するリスト
		std::array<bool , MRI::CommonConstant::k_vkCodeMaxNum> m_nowInputStateList = {};
		std::array<bool , MRI::CommonConstant::k_vkCodeMaxNum> m_oldInputStateList = {};

		HWND m_hWND = nullptr;

		// マウスの入力状態、座標を舞フレーム格納
		MRI::CommonStruct::Mouse m_nowMouse = {};
		MRI::CommonStruct::Mouse m_oldMouse = {};

		//===============================
		// シングルトン
		//===============================
		friend class MRI::SingletonBase<MRI::InputManager>;
		
		InputManager ()          = default;
		~InputManager() override = default;
	};
}