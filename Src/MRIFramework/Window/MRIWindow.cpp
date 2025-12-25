#include "MRIWindow.h"

// ウィンドウズメッセージを"ImGui"に渡すための準備
LRESULT ImGui_ImplWin32_WndProcHandler(HWND   a_hWND    , 
									   UINT   a_message , 
									   WPARAM a_wParam  , 
									   LPARAM a_lParam);

MRI::Window::Window () = default;
MRI::Window::~Window()
{

}

bool MRI::Window::Create(const int          a_clientWidth    , 
						 const int          a_clientHeight   , 
						 const std::string& a_titleName      , 
						 const std::string& a_windowClassName)
{
	// 現在の実行ファイルのハンドルを返す
	HINSTANCE l_hInst = GetInstanceHandle();

	// マルチバイト文字列に文字列を変換
	std::wstring l_wndClassName = sjis_to_wide(a_windowClassName);

	// ウィンドウクラスの定義
	WNDCLASSEX l_wc = {};

	l_wc.cbSize        = sizeof(WNDCLASSEX);						// 構造体のサイズを設定
	l_wc.style         = k_windowStyleDefault;					// ウィンドウの動作スタイル(サイズ変更時に再描画など)			
	l_wc.lpfnWndProc   = &MRI::Window::CallWindowProcedure;		// 関数を指定し指定した関数がウィンドウに届いたすべてのメッセージを処理
	l_wc.cbClsExtra    = k_classExtraBytes;						// ウィンドウクラス用に追加で確保するメモリサイズ
	l_wc.cbWndExtra    = k_windowExtraBytes;						// ウィンドウごとに追加で確保するメモリサイズ
	l_wc.hInstance     = l_hInst;								// アプリケーションのインスタンスハンドル
	l_wc.hIcon         = LoadIcon  (nullptr , IDI_APPLICATION); // ウィンドウに表示するアイコン
	l_wc.hIconSm       = LoadIcon  (nullptr , IDI_APPLICATION); // タスクバーやタイトルバーに表示されるアイコン
	l_wc.hCursor       = LoadCursor(nullptr , IDC_ARROW);		// ウィンドウ上で使うマウスカーソルを指定
	l_wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ウィンドウの背景色を指定
	l_wc.lpszMenuName  = nullptr;								// メニューリソースの名前
	l_wc.lpszClassName = l_wndClassName.c_str();				// ウィンドウクラス名

	// ウィンドウクラスを"OS"に登録
	if (!RegisterClassEx(&l_wc))
	{
		return false;
	}

	// ウィンドウの作成
	m_hWND = CreateWindow(l_wndClassName.c_str()                   ,	 // 登録済みのウィンドウクラス名
						  sjis_to_wide(a_titleName.data()).c_str() , // ウィンドウのタイトル(ワイド文字列に変換したもの)
						  WS_OVERLAPPEDWINDOW - WS_THICKFRAME      , // 標準ウィンドウからサイズ変更用の太い枠を除いたスタイル
						  k_defaultWindowPosX					   , // ウィンドウ座標"X"
						  k_defaultWindowPosY					   , // ウィンドウ座標"Y"
						  a_clientWidth							   , // クライアント領域の幅
						  a_clientHeight                           , // クライアント領域の高さ
						  nullptr								   , // 親ウィンドウがない
					      nullptr								   , // メニューなし
						  l_hInst								   , // アプリケーションのインスタンスハンドル
						  this);									 // 作製するウィンドウに"this"ポインタを渡す、"WM_CREATE"などで取り出せる
	if (!m_hWND)
	{
		// メモリリーク防止
		UnregisterClass(l_wndClassName.c_str() , l_hInst);
		return false;
	}
	
	// クライアントのサイズを設定
	SetClientSize(a_clientWidth , a_clientHeight);

	// ウィンドウの表示
	ShowWindow(m_hWND , SW_SHOW);
	// ウィンドウのクライアント領域をすぐに再描画
	UpdateWindow(m_hWND);

	// "timeGetTime"関数の精度を"1ms"に設定
	timeBeginPeriod(k_timerResolutionMS);

	return true;
}

LRESULT CALLBACK MRI::Window::CallWindowProcedure(const HWND   a_hWND    , 
												  const UINT   a_message , 
												  const WPARAM a_wParam  , 
												  const LPARAM a_lParam)
{
	auto* l_this = static_cast<MRI::Window*>(GetProp(a_hWND , L"GameWindowInstance"));

	// "nullptr"の場合はデフォルト処理を実行
	if (!l_this)
	{
		if (a_message == WM_CREATE)
		{
			// "CreateWindow"で渡したパラメータを取得
			auto* l_createStruct = (CREATESTRUCT*)a_lParam;
			auto* l_window       = (MRI::Window*)l_createStruct->lpCreateParams;

			// ウィンドウプロパティにこのクラスのインスタンスアドレスを埋め込んでおく
			// 次回から、"l_this->WindowProcedure"の方へ処理が流れていく
			SetProp(a_hWND , L"GameWindowInstance" , l_window);
		}
		else
		{
			return DefWindowProc(a_hWND    , 
								 a_message ,
								 a_wParam  ,
								 a_lParam);	
		}
	}

	// インスタンス側の"Window"関数を実行する
	return l_this->WindowProcedure(a_hWND    ,
								   a_message ,
								   a_wParam  ,
								   a_lParam);
}

LRESULT MRI::Window::WindowProcedure(const HWND   a_hWND    , 
								     const UINT   a_message , 
									 const WPARAM a_wParam  , 
									 const LPARAM a_lParam)
{

}