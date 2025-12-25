#include "main.h"

int WINAPI WinMain(_In_     HINSTANCE ,
				   _In_opt_ HINSTANCE ,
				   _In_     LPSTR     ,
				   _In_     int)
{
	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | 
				   _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr , COINIT_MULTITHREADED)))
	{
		CoUninitialize();
		return 0;
	}

	// mbstowcs_s関数で日本語対応にするために呼ぶ
	setlocale(LC_ALL , "japanese");

	Application::GetInstance().Execute();

	// COM解放
	CoUninitialize();
	return 0;
}

void Application::Execute()
{
	// ウィンドウの解像度を読み取る
	LoadWindowSize();

	// 初期設定(ウィンドウ作成、Direct3D初期化など)
	if (!Application::GetInstance().Init(m_windowSize))
	{
		return;
	}

	// ゲームループ
	m_fpsController.Init();

	while (true)
	{
		if (m_isEndGameLoop) { break; }

		// "FPS"の計測
		m_fpsController.UpdateStartTime();

		// ウィンドウメッセージの処理
		m_gameWindow.ProcessMessage();

		// ウィンドウが破棄されているか"Escape"キーが押されていたらゲームループ終了
		if (!m_gameWindow.IsCreated()) 
		{
			EndGameLoop();
		}

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			EndGameLoop();
		}

		// アプリケーション更新
		BeginUpdate();
		EarlyUpdate();
		Update     ();
		LateUpdate ();
		EndUpdate  ();

		// アプリケーション描画更新

		// フレームレート制御
		m_fpsController.Update();

		// 一フレーム前の入力の確認
		MRI::InputManager::GetInstance().BackUpInputState();

		// タイトル名 + "FPS"の表示
		const std::string l_titleBar = GetTitleBarWithFPS();
		SetWindowTextA                                   (GetHWND(), l_titleBar.c_str());
	}

	// ウィンドウの解像度を保存(ウィンドウサイズの設定は保存しておくべきだから)
	SaveWindowSize();

	// アプリケーション開放
	Release();
}

void Application::EndGameLoop()
{
	m_isEndGameLoop = true;
}

bool Application::Init(const MRI::CommonStruct::Size& a_size)
{
	// タイトル名 + "FPS"の表示
	const std::string l_titleBar = GetTitleBarWithFPS();
	SetWindowTextA									 (GetHWND() , l_titleBar.c_str());

	if (!m_gameWindow.Create(a_size     ,
						     l_titleBar , 
						     "Window")) 
	{
		MessageBoxA(nullptr                , 
					"ウィンドウ作成に失敗" , 
					"エラー"               ,
					MB_OK);

		return false;
	}

	// フルスクリーン
	bool l_isFullScreen = false;
	//	if (MessageBoxA(m_window.GetWndHandle(), "フルスクリーンにしますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
	//		bFullScreen = true;
	//	}

	// デバイスのデバッグモードを有効にする
	bool l_deviceDebugMode = false;
#ifdef _DEBUG
	l_deviceDebugMode = true;
#endif

	// "Direct3D"初期化
	if (std::string l_errorMsg = k_direct3DErrorMessage.data();
	!KdDirect3D::Instance().Init(m_gameWindow.GetHWND() ,
								 a_size.width           , 
								 a_size.height          , 
								 l_deviceDebugMode      , 
								 l_errorMsg))
	{
		MessageBoxA(m_gameWindow.GetHWND() , 
					l_errorMsg.c_str    () , 
					"Direct3D初期化失敗"   , 
					MB_OK				   |
					MB_ICONSTOP);

		return false;
	}

	// フルスクリーン設定
	if (l_isFullScreen) 
	{	
		HRESULT l_hr = {};

		l_hr = KdDirect3D::Instance().SetFullscreenState(TRUE , nullptr);
		if (FAILED(l_hr))
		{
			MessageBoxA(m_gameWindow.GetHWND()	 , 
						"フルスクリーン設定失敗" , 
						"Direct3D初期化失敗"     , 
						MB_OK					 | 
						MB_ICONSTOP);

			return false;
		}
	}

	// "InputManager"の初期化
	auto& l_inputManager = MRI::InputManager::GetInstance();
	l_inputManager.SetHWND(GetHWND());
	l_inputManager.Init   ();

	// シェーダー初期化
	KdShaderManager::Instance().Init();

	// オーディオ初期化
	KdAudioManager::Instance().Init();

	m_isEndGameLoop = false;
	
	return false;
}

void Application::LoadWindowSize()
{
	auto l_rootJson = MRI::FileIOUtility::LoadJsonFile(k_windowSizeFileIOPath.data());
	if (l_rootJson.is_null()) { return; }

	m_windowSize.height = l_rootJson.value("Height" , MRI::CommonConstant::k_defaultWindowHeight);
	m_windowSize.width  = l_rootJson.value("Width"  , MRI::CommonConstant::k_defaultWindowWidth);
}
void Application::SaveWindowSize()
{
	auto l_rootJson = nlohmann::json();

	l_rootJson["Height"] = m_windowSize.height;
	l_rootJson["Width"]  = m_windowSize.width;

	MRI::FileIOUtility::SaveJsonFile(l_rootJson , k_windowSizeFileIOPath.data());
}

void Application::BeginUpdate()
{
	// 空間環境の更新
	KdShaderManager::Instance().WorkAmbientController().Update();
}
void Application::EarlyUpdate() const
{
}
void Application::Update() const
{
}
void Application::LateUpdate() const
{
}
void Application::EndUpdate() const
{
	// "3DSoundListener"の行列を更新
	KdAudioManager::Instance().SetListnerMatrix(KdShaderManager::Instance().GetCameraCB().mView.Invert());
}

void Application::Release()
{
	KdInputManager::Instance().Release ();
	KdShaderManager::Instance().Release();
	KdAudioManager::Instance().Release ();
	KdDirect3D::Instance().Release	   ();

	// ウィンドウ削除
	m_gameWindow.Release();
}

std::string Application::GetTitleBarWithFPS() const
{
	return std::format("{} : {}" , k_titleName , GetNowFPS());
}