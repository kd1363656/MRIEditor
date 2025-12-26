#include "MRIEditorGameView.h"

#include "../../../../Application/main.h"

void MRI::Editor::EditorGameView::Init()
{
	const MRI::CommonStruct::WindowSize l_windowSize = Application::GetInstance().GetWindowSize();

	// レンダーターゲットを作成
	m_renderTargetPack.CreateRenderTarget(l_windowSize.width , l_windowSize.height , true);
}

void MRI::Editor::EditorGameView::DrawEditor()
{
	if (!ImGui::Begin("GameView"))
	{
		ImGui::End();
		return;
	}
	
	DrawSceneSelector();
	DrawScene        ();
	ImGui::End       ();
}

void MRI::Editor::EditorGameView::RenderTargetClearTex()
{
	// 前のバックバッファの情報をクリア
	m_renderTargetPack.ClearTexture();
}
void MRI::Editor::EditorGameView::ChangeRenderTarget()
{
	// 描画先をバックバッファからレンダーターゲットに切り替えて保存
	m_renderTargetChanger.ChangeRenderTarget(m_renderTargetPack);
}
void MRI::Editor::EditorGameView::UndoRenderTarget()
{
	// レンダーターゲットの切り替えを修理牛手
	// 描画先をバックバッファに戻す
	m_renderTargetChanger.UndoRenderTarget();
}

void MRI::Editor::EditorGameView::DrawSceneSelector()
{
	auto& l_sceneManager = SceneManager::GetInstance																									  ();
	auto  l_sceneCache   = l_sceneManager.GetSceneCache().lock																							  ();
	auto  l_editorCache  = MRI::Editor::EditorManager::GetInstance().FetchEditorViewCache<MRI::Editor::EditorPrefabSceneShiftRegistryInspectorView>().lock();
	
	if (!l_sceneCache || 
		!l_editorCache) 
	{
		return; 
	}

	ImGui::SetNextItemWidth(MRI::EditorCommonConstant::k_comboWidth);
	ImGui::BeginGroup      ();

	// もしコンボボックスが開かれていなければ早期リターン
	if (!ImGui::BeginCombo("SceneSelector" , "SceneSelector"))
	{
		ImGui::EndGroup();
		return;
	}

	auto&       l_sceneShiftMap    = l_editorCache->GetAllSceneShiftMap();
	const auto& l_typeInfoRegistry = MRI::TypeInfoRegistry::GetInstance();

	// シーンチェンジしたいシーンを選びそれに該当するファイルパスをシーンマネージャーに読み込ませ
	for (const auto& [l_key , l_value] : l_sceneShiftMap)
	{
		const auto* l_typeInfo = l_typeInfoRegistry.FetchTypeInfoByID(l_key);
		if (!l_typeInfo) { continue; }

		ImGui::PushID(&l_key);

		bool l_isSelected = false;

		if (!ImGui::RadioButton(l_typeInfo->k_name.data() , l_isSelected))
		{
			ImGui::PopID();
			continue;
		}
		
		if (l_isSelected)
		{
			ImGui::SetItemDefaultFocus();
		}

		// ファイルパスを読み込みシーン遷移を行う
		l_sceneManager.SceneShift(l_value);
		
		ImGui::PopID();
	}

	ImGui::EndCombo();
	ImGui::EndGroup();
}
void MRI::Editor::EditorGameView::DrawScene()
{
	// 描画がレンダーターゲットパックに保存されていなければ"return"
	auto l_rtTex = m_renderTargetPack.m_RTTexture;
	if (!l_rtTex) { return; }

	// テクスチャ"ID"に変換
	const auto l_texID = (ImTextureID)l_rtTex->GetSRView();

	// ウィンドウの描画可能な領域サイズを取得
	const ImVec2 l_availableSize = ImGui::GetContentRegionAvail();

	// アスペクト比"16 : 9"をもとに描画サイズを調整
	// "高さ / 横幅"で倍率を取得
	const float l_aspect    = k_generalAspectRation.y / k_generalAspectRation.x;
	ImVec2      l_imageSize = {};

	// 縦をアスペクト比に合わせる
	l_imageSize.x = l_availableSize.x;
	l_imageSize.y = l_availableSize.x * l_aspect;

	// 高さが足りない場合は縦基準に切り替え
	if (l_imageSize.y > l_availableSize.y)
	{
		l_imageSize.y = l_availableSize.y;
		l_imageSize.x = l_imageSize.y / l_aspect;
	}

	// テクスチャ描画
	ImGui::Image(l_texID , l_imageSize);
}