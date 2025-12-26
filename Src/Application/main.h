#pragma once

//============================================================
// アプリケーションクラス
//	APP.～ でどこからでもアクセス可能
//============================================================
class Application final : public MRI::SingletonBase<Application>
{
// メンバ
public:

	// アプリケーション実行
	void Execute    ();
	void EndGameLoop();

	HWND GetHWND() const { return m_gameWindow.GetHWND(); }

	int GetNowFPS() const { return m_fpsController.GetNowFPS(); }

private:

	bool Init(const MRI::CommonStruct::Size& a_size);

	void LoadWindowSize();
	void SaveWindowSize();

	void BeginUpdate();
	void EarlyUpdate() const;
	void Update     () const;
	void LateUpdate () const;
	void EndUpdate  () const;

	void Release();

	void DrawWindowTitleBar();

	std::string GetTitleBarWithFPS() const;

	static constexpr std::string_view k_direct3DErrorMessage   = "Direct3D初期化失敗";
	static constexpr std::string_view k_fullScreenErrorMessage = "フルスクリーン設定失敗";

	static constexpr std::string_view k_titleName            = "MRI_Editor";
	static constexpr std::string_view k_windowSizeFileIOPath = "Asset/Data/Setting/Window/WindowConfig.json";

	MRI::FPSController m_fpsController = {};

	MRI::Window m_gameWindow = {};

	MRI::CommonStruct::Size m_windowSize = {};

	bool m_isEndGameLoop = false;

	//===============================
	// シングルトン
	//===============================
	friend class MRI::SingletonBase<Application>;

	Application ()          = default;
	~Application() override = default;
};
