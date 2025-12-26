#include "MRIEditorManager.h"

#include "../../Application/main.h"

MRI::Editor::EditorManager::EditorManager ()
{
	InitEditor();
}
MRI::Editor::EditorManager::~EditorManager()
{
	ReleaseEditor();
}

void MRI::Editor::EditorManager::Init()
{
	// 各ウィンドの生成と初期化
	CreateEditorView<MRI::Editor::EditorGameView>                             ();
	CreateEditorView<MRI::Editor::EditorHierarchyView>                        ();
	CreateEditorView<MRI::Editor::EditorPrefabGameObjectRegistryInspectorView>();
	CreateEditorView<MRI::Editor::EditorSpawnSceneLoadPrefabInspectorView>    ();
	CreateEditorView<MRI::Editor::EditorPrefabGameObjectInspectorView>        ();
	CreateEditorView<MRI::Editor::EditorSpawnGameObjectInspectorView>         ();
	CreateEditorView<MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView>();
	CreateEditorView<MRI::Editor::EditorSpawnSceneShiftInspectorView>         ();
	CreateEditorView<MRI::Editor::EditorPlayerKeyConfigInspectorView>         ();

	for (const auto& l_view : m_editorViewList)
	{
		if (!l_view) { continue; }
		l_view->Init();
	}

	if (!m_editorMenuBar)
	{
		m_editorMenuBar = std::make_unique<MRI::Editor::EditorMenuBar>();
	}

	m_editorMenuBar->Init();

	LoadFile();
}
void MRI::Editor::EditorManager::DrawEditor()
{
	// ログウィンドウがインスタンス化されていなければ実行しない
	if (!m_editorLogView) { return; }

	// =============================================================================
	// "ImGui"の開始
	// =============================================================================
	ImGui_ImplDX11_NewFrame ();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame         ();

	DrawDockSpace	();
	DrawEditorWindow();
	PostDrawEditor  ();
}

void MRI::Editor::EditorManager::LoadFile() const
{
	for (const auto& l_view : m_editorViewList)
	{
		if (!l_view) { continue; }
		l_view->LoadFile();
	}
}
void MRI::Editor::EditorManager::SaveFile() const
{
	for (const auto& l_view : m_editorViewList)
	{
		if (!l_view) { continue; }
		l_view->SaveFile();
	}
}

void MRI::Editor::EditorManager::AddLog(const char* a_format , ...)
{
	// ログウィンドウがインスタンス化されていなければ実行しない
	if (!m_editorLogView) { return; }

	char l_logBuffer[k_logBufferSize];

	va_list l_args;
	va_start(l_args , a_format);

	vsprintf_s(l_logBuffer , a_format , l_args);

	va_end				   (l_args);
	m_editorLogView->AddLog(l_logBuffer);
}

void MRI::Editor::EditorManager::InitEditor()
{
	// ログウィンドウがインスタンス化されていたら実行しない
	if (m_editorLogView) { return; }

	// =============================================================================
	// "ImGui"のバージョンをチェックし"ImGui"と"ImNodes"のセットアップをする
	// =============================================================================
	IMGUI_CHECKVERSION    ();
	ImGui::CreateContext  ();
	ImNodes::CreateContext();

	// =============================================================================
	// "ImGuiDocking"の導入
	// =============================================================================
	ImGuiIO& l_io = ImGui::GetIO();

	// "ImGui"をウィンドウの外側に配置しても独立した"OS"ウィンドウとして
	// 表示を可能に、ウィンドウを結合、分割を可能にするフラグを合成する
	l_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	l_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// =============================================================================
	// "ImGui"のカラーを設定
	// =============================================================================
	ImGui::StyleColorsDark();

	// =============================================================================
	// "ImGui"の作動環境を設定("OS"と"描画"API")
	// =============================================================================
	ImGui_ImplWin32_Init(Application::GetInstance().GetHWND());
	ImGui_ImplDX11_Init (KdDirect3D::Instance    ().WorkDev() , KdDirect3D::Instance().WorkDevContext());

	// =============================================================================
	// "ImGui"のフォントを設定(英語と日本語)
	// =============================================================================
#include "imgui/ja_glyph_ranges.h"
	
	ImFontConfig l_config;

	l_config.MergeMode = true;

	l_io.Fonts->AddFontDefault();
	// 日本語対応
	l_io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc" , 
								   k_fontSize						  ,
								   &l_config                          ,
								   glyphRangesJapanese);

	// =============================================================================
	// "LogView"の初期化
	// =============================================================================
	if (!m_editorLogView)
	{
		m_editorLogView = std::make_unique<MRI::Editor::EditorLogView>();
	}

	m_editorLogView->Init();
}
void MRI::Editor::EditorManager::ReleaseEditor() const
{
	// ログウィンドウがインスタンス化されていなければ実行しない
	if (!m_editorLogView) { return; }

	ImGui_ImplDX11_Shutdown ();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext   ();
	ImNodes::DestroyContext ();
}

void MRI::Editor::EditorManager::DrawDockSpace() const
{
	// =============================================================================
	// "ImGuiDockingSpace"の作製
	// =============================================================================
	
	// "DockSpace"全体を包むルートウィンドウ(親ウィンドウ)を画面サイズで作成
	ImGuiWindowFlags l_windowFlag = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	const ImGuiViewport* l_viewPort	= ImGui::GetMainViewport();

	// メインウィンドウを取得してサイズ、描画位置を現在のウィンドウサイズに合わせる
	ImGui::SetNextWindowPos     (l_viewPort->WorkPos );
	ImGui::SetNextWindowSize    (l_viewPort->WorkSize);
	ImGui::SetNextWindowViewport(l_viewPort->ID      );

	// 親ウィンドウとして振舞うためウィンドウ名を表示しない、折り畳みができない
	// 移動しない、他ウィンドウがこのウィンドウより上に描画されるようにする、
	// キーボードナビゲーションでこのウィンドウにフォーカスがいかないようにする
	l_windowFlag |= ImGuiWindowFlags_NoTitleBar            | 
					ImGuiWindowFlags_NoCollapse            | 
					ImGuiWindowFlags_NoMove                |
					ImGuiWindowFlags_NoBringToFrontOnFocus |
					ImGuiWindowFlags_NoNavFocus;

	// 親ウィンドウを角丸・枠なしにする設定
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding   , 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize , 0.0f);
	ImGui::Begin       ("DockSpace"                    , nullptr  , l_windowFlag);
	ImGui::PopStyleVar (2);

	// "DockSpace"の作成
	ImGuiID l_dockSpaceID = ImGui::GetID("DockSpace");
	ImVec2  l_size        = {};
	ImGui::DockSpace(l_dockSpaceID , l_size , ImGuiDockNodeFlags_None);

	ImGui::End();
}
void MRI::Editor::EditorManager::DrawEditorWindow()
{
	for (const auto& l_view : m_editorViewList)
	{
		if (!l_view) { continue; }
		l_view->DrawEditor();
	}

	if (m_editorMenuBar)
	{
		m_editorMenuBar->DrawEditor();
	}

	if (m_editorLogView)
	{
		m_editorLogView->DrawEditor();
	}

	// ヘルプの表示
	//ImGui::ShowDemoWindow();
}
void MRI::Editor::EditorManager::PostDrawEditor()
{
	// =============================================================================
	// ここより上で"ImGui"の描画はする事
	// =============================================================================
	ImGui::Render				 ();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// "ImGuiDocking"のマルチウィンドウ処理
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		// 描画、更新
		ImGui::UpdatePlatformWindows       ();
		ImGui::RenderPlatformWindowsDefault();

		auto& l_kdDirect3D = KdDirect3D::Instance();

		// レンダーターゲットを戻す
		l_kdDirect3D.WorkDevContext()->OMSetRenderTargets(k_numView , l_kdDirect3D.GetBackBuffer()->WorkRTViewAddress() , l_kdDirect3D.GetZBuffer()->WorkDSView());
	}
}