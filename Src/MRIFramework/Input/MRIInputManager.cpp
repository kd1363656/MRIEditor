#include "MRIInputManager.h"

#include "Application/main.h"

void MRI::InputManager::Init()
{
	m_nowInputStateList.fill(false);
	m_oldInputStateList.fill(false);

	RegisterDevice();
}
void MRI::InputManager::RegisterDevice()
{
	const auto& l_application = Application::GetInstance();

	// m_hWNDがしっかりセットされていれば実行
	assert(l_application.GetHWND() != nullptr);

	const auto l_indexMax = static_cast<std::size_t>(MRI::InputManager::DeviceIndex::DeviceCount);
	RAWINPUTDEVICE l_rid[l_indexMax];
	
	// デバイスの登録を行っていく
	// キーボード
	{
		const auto l_index = static_cast<std::size_t>(MRI::InputManager::DeviceIndex::KeyBoard);
		Register(l_rid[l_index]     , 
				 k_usagePageGeneric , 
				 k_usageKeyBoard    , 
				 k_inputFlags);
	}

	// マウス
	{
		const auto l_index = static_cast<std::size_t>(MRI::InputManager::DeviceIndex::Mouse);
		Register(l_rid[l_index]     , 
				 k_usagePageGeneric ,
				 k_usageMouse       , 
				 k_inputFlags);
	}

	RegisterRawInputDevices(l_rid , l_indexMax , sizeof(RAWINPUTDEVICE));
}

void MRI::InputManager::ResetInputStates()
{
	m_nowInputStateList.fill(false);
	m_oldInputStateList.fill(false);
}

bool MRI::InputManager::IsInputHeld(const int a_vkCode) const
{
	// 仮想キーコードにない値なら"false"を返す
	if (a_vkCode >= MRI::CommonConstant::k_vkCodeMaxNum) 
	{
		return false;
	}

	return m_nowInputStateList[a_vkCode];
}

bool MRI::InputManager::IsInputOnce(const int a_vkCode) const
{
	// 仮想キーコードにない値なら"false"を返す
	if (a_vkCode >= MRI::CommonConstant::k_vkCodeMaxNum) 
	{
		return false; 
	}

	// もしキーがどちらも格納されていなければキーが初めて押されたことになるから
	// "true"を返す
	const bool l_isPressNow = m_nowInputStateList[a_vkCode];
	const bool l_isPressOld = m_oldInputStateList[a_vkCode];

	return l_isPressNow && !l_isPressOld;
}

void MRI::InputManager::ProcessInput(const LPARAM a_lParam)
{
	UINT l_dwSize = 0U;
	GetRawInputData((HRAWINPUT)a_lParam    , 
					 RID_INPUT			   , 
					 nullptr			   , 
					 &l_dwSize			   , 
					 sizeof(RAWINPUTHEADER));

	BYTE l_buffer[k_rawInputBufferSize];		 // 最大1024バイトまでの固定バッファにする

	if (l_dwSize == 0U             ) { return; } // 何もデータがないときはスキップ
	if (l_dwSize > sizeof(l_buffer)) { return; } // サイズを超えていたらスキップ

	if (const auto l_rawInput = GetRawInputData((HRAWINPUT)a_lParam    , 
										         RID_INPUT			   ,
										         l_buffer			   ,
										         &l_dwSize			   ,
										         sizeof(RAWINPUTHEADER));
		l_rawInput != l_dwSize) 
	{
		return;
	}
	
	auto* l_raw = reinterpret_cast<RAWINPUT*>(l_buffer);

	// キーボード
	if(l_raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		const int  l_key     = l_raw->data.keyboard.VKey;
		const bool l_pressed = !(l_raw->data.keyboard.Flags & RI_KEY_BREAK);

		// キーの入力状態を格納
		// 存在すればそのキーに対応する値を格納
		if (l_key < MRI::CommonConstant::k_vkCodeMaxNum)
		{
			m_nowInputStateList[l_key] = l_pressed;
		}
	}
	// マウス
	if(l_raw->header.dwType == RIM_TYPEMOUSE)
	{
		WORD l_flags = l_raw->data.mouse.usButtonFlags;

		// ボタンの状態を明示的に管理（押下/解放で変更）
		// "RawInput"の仕様上効かない時があるから
		// マウスの中央ボタンの状態
		UpdateMouseButtonState(l_flags					   ,
							   RI_MOUSE_MIDDLE_BUTTON_DOWN , 
							   RI_MOUSE_MIDDLE_BUTTON_UP   ,
							   VK_MBUTTON);

		// マウスの左ボタンの状態
		UpdateMouseButtonState(l_flags					 ,
							   RI_MOUSE_LEFT_BUTTON_DOWN ,
							   RI_MOUSE_LEFT_BUTTON_UP   ,
							   VK_LBUTTON);

		// マウスの右ボタンの状態
		UpdateMouseButtonState(l_flags                    , 
							   RI_MOUSE_RIGHT_BUTTON_DOWN ,
							   RI_MOUSE_RIGHT_BUTTON_UP   ,
							   VK_RBUTTON);

		// ホイール
		if(l_flags & RI_MOUSE_WHEEL)
		{
			m_nowMouse.wheelDelta = static_cast<SHORT>(l_raw->data.mouse.usButtonData) / WHEEL_DELTA;
		}
		else
		{
			// 回転がない場合は0
			m_nowMouse.wheelDelta = 0;
		}
	}
}

void MRI::InputManager::BackUpInputState()
{
	// 現在のフレームでのキー情報を格納する
	for(std::size_t l_ki = 0LLU; l_ki < MRI::CommonConstant::k_vkCodeMaxNum; l_ki++)
	{
		m_oldInputStateList[l_ki] = m_nowInputStateList[l_ki];
	}

	// マウスの情報のバックアップ
	m_oldMouse.pos        = m_nowMouse.pos;
	m_oldMouse.wheelDelta = m_nowMouse.wheelDelta;
}

Math::Vector3 MRI::InputManager::FetchMouseDeltaAndResetCursorCenter() const
{
	const auto& l_application = Application::GetInstance();

	// ウィンドウの半分の大きさ
	const MRI::CommonStruct::Dimension2D& l_windowHalfSize = { l_application.GetWindowSize().width  / 2 ,
															   l_application.GetWindowSize().height / 2 };

	// 今のマウス座標をスクリーン座標で取得(デスクトップの左上を基準)
	POINT l_nowMousePos = {};
	GetCursorPos(&l_nowMousePos);

	// ウィンドウハンドル(現在のウィンドウのハンドルを取得)
	const auto& l_hWND = l_application.GetHWND();

	// スクリーン座標からクライアント座標に変換(タイトルバーを含まない現在のウィンドウの左上を基準とした座標を取得)
	ScreenToClient(l_hWND , &l_nowMousePos);

	// マウスの移動量(現在のマウス位置から画面の中央座標を引いた差分)
	// "Z"軸に移動していないので"Z"軸は含めない
	Math::Vector3 l_movement = { static_cast<float>(l_nowMousePos.x - l_windowHalfSize.width)  ,
								 static_cast<float>(l_nowMousePos.y - l_windowHalfSize.height) ,
								 0.0F };

	// 移動量がほぼなければ"Math::Vector3::Zero"の値を返す
	if (l_movement.LengthSquared() <= MRI::CommonConstant::k_epsilon) 
	{
		return Math::Vector3();
	}

	// クライアント座標の中心をスクリーン座標に変換
	POINT l_centerScreenPos = { l_windowHalfSize.width , l_windowHalfSize.height };

	// クライアント座標の中心をスクリーン座標に変換
	ClientToScreen(l_hWND , &l_centerScreenPos);

	// マウスをクライアント画面中心に戻す
	SetCursorPos(l_centerScreenPos.x , l_centerScreenPos.y);
	
	return l_movement;
}

void MRI::InputManager::Register(RAWINPUTDEVICE& a_rid       ,
								 const USHORT    a_usagePage , 
								 const USHORT	 a_usage     , 
								 const DWORD		 a_dwFlags)
{
	const auto& l_application = Application::GetInstance();

	a_rid.usUsagePage = a_usagePage; //標準デバイス
	a_rid.usUsage	  = a_usage;    
	a_rid.dwFlags	  = a_dwFlags;		
	a_rid.hwndTarget  = l_application.GetHWND();		
}

void MRI::InputManager::UpdateMouseButtonState(const WORD a_flags    , 
											   const WORD a_downFlag , 
											   const WORD a_upFlag   , 
											   const int  a_vkCode)
{
	if (a_flags & a_downFlag)
	{
		m_nowInputStateList[a_vkCode] = true;
	}
	else if (a_flags & a_upFlag)
	{
		m_nowInputStateList[a_vkCode] = false;
	}
}