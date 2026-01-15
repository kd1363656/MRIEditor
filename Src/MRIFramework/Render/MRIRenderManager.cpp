#include "MRIRenderManager.h"

void MRI::RenderManager::Init()
{
	m_mainCameraComponentCache.reset();
	
	m_renderModelComponentBaseCacheList.clear();
}
void MRI::RenderManager::PostLoadInit()
{
	// リストの保持やメインカメラの保持をやめる
	Reset();

	auto l_sceneCache = SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	// ゲームオブジェクトからカメラコンポーネントを取得
	for (const auto& l_gameObject : l_sceneCache->GetGameObjectList())
	{
		if (!l_gameObject) { continue; }

		auto l_cameraComponentCache = l_gameObject->GetComponentCache<MRI::Component::CameraComponent>().lock();
		if (!l_cameraComponentCache) { continue; }

		// メインカメラの属性を持つコンポーネントのみ取得
		if (l_cameraComponentCache->GetCameraTag() != MRI::GetTypeInfo<MRI::Tag::CameraMainTag>().k_id) { continue; }
		m_mainCameraComponentCache = l_cameraComponentCache;
	}

	// ゲームオブジェクトからモデル描画コンポーネントを取得
	for (const auto& l_gameObject : l_sceneCache->GetGameObjectList())
	{
		if (!l_gameObject) { continue; }

		for (auto l_component : l_gameObject->GetComponentList())
		{
			if (!l_component) { continue; }

			// もし"renderModelComponentCache"がうまくキャストできなければ空の"std::weak_ptr<MRI::Component::renderModelComponentBase>"が帰ってくる
			auto l_renderModelComponentCache = MRI::TypeInfoUtility::SafeCast<MRI::Component::RenderModelComponentBase>(l_component);
			if (l_renderModelComponentCache.expired()) { continue; }

			m_renderModelComponentBaseCacheList.emplace_back(l_renderModelComponentCache);
		}
	}
}

// 描画開始
void MRI::RenderManager::BeginDraw(const bool a_usePostProcess) const
{
	KdDirect3D::Instance().ClearBackBuffer                  ();
	KdShaderManager::Instance().WorkAmbientController().Draw();

	if (!a_usePostProcess) return;
	KdShaderManager::Instance().m_postProcessShader.Draw();
}

// 描画の前処理
void MRI::RenderManager::PreDraw() const
{
	const auto l_editorGameView			  = MRI::Editor::EditorManager::GetInstance().FetchEditorViewCache<MRI::Editor::EditorGameView>().lock();
	const auto l_mainCameraComponentCache = m_mainCameraComponentCache.lock																	  ();

	if (!l_editorGameView || !l_mainCameraComponentCache) { return; }

	l_editorGameView->RenderTargetClearTex();
	l_editorGameView->ChangeRenderTarget  ();

	// カメラ描画
	l_mainCameraComponentCache->PreDraw();
	
	// 描画をバックバッファーに戻す
	l_editorGameView->UndoRenderTarget();
}

// 描画
void MRI::RenderManager::Draw() const
{
	const auto l_editorGameView = MRI::Editor::EditorManager::GetInstance().FetchEditorViewCache<MRI::Editor::EditorGameView>().lock();
	if (!l_editorGameView) { return; }

	l_editorGameView->ChangeRenderTarget();

	// 光を遮るオブジェクト(不透明な物体や"2D"キャラ)は"Begin"と"End"の間にまとめて"Draw"する
	KdShaderManager::Instance().m_StandardShader.BeginGenerateDepthMapFromLight();
	{
		DrawModel<MRI::Tag::ShaderTagGenerateDepthMapFromLight>();
	}
	KdShaderManager::Instance().m_StandardShader.EndGenerateDepthMapFromLight();

	// 陰影のないオブジェクト(背景など)は"Begin"と"End"の間にまとめて"Draw"する
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		DrawModel<MRI::Tag::ShaderTagUnLit>();
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	// 陰影のあるオブジェクト(不透明な物体や"2D"キャラ)は"Begin"と"End"の間にまとめて"Draw"する
	KdShaderManager::Instance().m_StandardShader.BeginLit();
	{
		DrawModel<MRI::Tag::ShaderTagLit>();
	}
	KdShaderManager::Instance().m_StandardShader.EndLit();

	// 陰影のないオブジェクト(エフェクトなど)は"Begin"と"End"の間にまとめて"Draw"する
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		DrawModel<MRI::Tag::ShaderTagEffect>();
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)は"Begin"と"End"の間にまとめて"Draw"する
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
		DrawModel<MRI::Tag::ShaderTagBright>();
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();

	l_editorGameView->UndoRenderTarget();
}

// 描画の後処理
void MRI::RenderManager::PostDraw() const
{
	// 画面のぼかしや被写界深度処理の実施
	KdShaderManager::Instance().m_postProcessShader.PostEffectProcess();

	MRI::RenderManager::GetInstance().DrawDebug();
}

// 2Dスプライトの描画
void MRI::RenderManager::DrawSprite() const
{
	const auto l_editorGameView = MRI::Editor::EditorManager::GetInstance().FetchEditorViewCache<MRI::Editor::EditorGameView>().lock();
	if (!l_editorGameView) { return; }

	l_editorGameView->ChangeRenderTarget();

	// "2D"の描画はこの間で行う
	KdShaderManager::Instance().m_spriteShader.Begin();
	{

	}
	KdShaderManager::Instance().m_spriteShader.End();

	l_editorGameView->UndoRenderTarget();
}

void MRI::RenderManager::DrawDebug() const
{
	const auto l_editorGameView = MRI::Editor::EditorManager::GetInstance().FetchEditorViewCache<MRI::Editor::EditorGameView>().lock();
	if (!l_editorGameView) { return; }

	l_editorGameView->ChangeRenderTarget();

	// デバッグ情報の描画はこの間で行う
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	l_editorGameView->UndoRenderTarget();
}

// アプリケーション描画終了 
void MRI::RenderManager::EndDraw() const
{
	// "Editor"のレンダリング
	MRI::Editor::EditorManager::GetInstance().DrawEditor();

	// "BackBuffer" "->" 画面表示
	KdDirect3D::Instance().WorkSwapChain()->Present(0, 0);
}

void MRI::RenderManager::Reset()
{
	m_mainCameraComponentCache.reset();

	m_renderModelComponentBaseCacheList.clear();
}