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
	void Execute();

	int GetNowFPS() const { return m_fpsController.GetNowFPS(); }

private:

	bool Init(const int a_w , const int a_h);

	std::string GetTitleBarWithFPS() const;

	static constexpr std::string_view k_titleName = "MRI_Editor";

	MRI::FPSController m_fpsController = {};

	//===============================
	// シングルトン
	//===============================
	friend class MRI::SingletonBase<Application>;

	Application ()          = default;
	~Application() override = default;
};
