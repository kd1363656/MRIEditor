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

}

bool Application::Init(const int a_w, const int a_h)
{
	// タイトル名 + "FPS"の表示
	const std::string l_titleBar = GetTitleBarWithFPS();
	SetWindowTextA									 (GetWidnowHandle() , l_titleBar.c_str());

	return false;
}

std::string Application::GetTitleBarWithFPS() const
{
	return std::format("{} : {}" , k_titleName , GetNowFPS());
}