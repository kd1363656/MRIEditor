#pragma once

namespace MRI
{
	class Window
	{
	public:

		Window () = default;
		~Window() = default;

		bool Create(const MRI::CommonStruct::Size& a_size			  ,
					const std::string&			   a_titleName        ,
					const std::string&			   a_windowClassName);

		bool ProcessMessage() const;

		void Release();

		bool IsCreated() const { return m_hWND ? true : false; }

		HWND GetHWND() const { return m_hWND; }

		HINSTANCE GetInstanceHandle() const { return GetModuleHandle(nullptr); }

	private:

		void SetClientSize(const MRI::CommonStruct::Size& a_size);

		static constexpr UINT k_timerResolutionMS = 1U;

		static constexpr UINT k_messageFilterNone = 0u;

		static constexpr UINT k_windowStyleDefault = 0U;

		static constexpr int k_classExtraBytes  = 0;
		static constexpr int k_windowExtraBytes = 0;

		static constexpr int k_defaultWindowPosX = 0;
		static constexpr int k_defaultWindowPosY = 0;

		HWND m_hWND = nullptr;

		// ウィンドウ関数
		static LRESULT CALLBACK CallWindowProcedure(const HWND   a_hWND    , 
													const UINT   a_message , 
													const WPARAM a_wParam  , 
													const LPARAM a_lParam);

		LRESULT CALLBACK WindowProcedure(const HWND   a_hWND    , 
										 const UINT   a_message , 
										 const WPARAM a_wParam  , 
										 const LPARAM a_lParam);
	};
}