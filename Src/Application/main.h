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

	const MRI::CommonStruct::Dimension2D& GetWindowSize() const { return m_windowSize; }
	
	HWND GetHWND() const { return m_gameWindow.GetHWND(); }

	int GetNowFPS() const { return m_fpsController.GetNowFPS(); }

	float GetScaledDeltaTime() const { return m_fpsController.GetScaledDeltaTime(); }

private:

	bool Init(const MRI::CommonStruct::Dimension2D& a_size);

	void LoadWindowSize();
	void SaveWindowSize();

	void BeginUpdate() const;
	void EarlyUpdate() const;
	void PreUpdate  () const;
	void Update     () const;
	void LateUpdate () const;
	void FixMatrix  () const;
	void EndUpdate  () const;

	void Release();

	void UpdateWindowTitleBar() const;

	std::string GenerateWindowTitleText() const;

	static constexpr std::string_view k_direct3DErrorMessage   = "Direct3D初期化失敗";
	static constexpr std::string_view k_fullScreenErrorMessage = "フルスクリーン設定失敗";

	static constexpr std::string_view k_titleName = "MRI_Editor";

	const std::string k_windowSizeFileIOPath = "Asset/Data/Setting/Window/WindowConfig.json";
	const std::string k_firstSceneFilePath   = "Asset/Data/Scene/Game/GameScene.json";

	MRI::FPSController m_fpsController = {};

	MRI::Window m_gameWindow = {};

	MRI::CommonStruct::Dimension2D m_windowSize = {};

	bool m_isEndGameLoop = false;

	//===============================
	// シングルトン
	//===============================
	friend class MRI::SingletonBase<Application>;

	Application ()          = default;
	~Application() override = default;
};
