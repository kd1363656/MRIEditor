#include "MRIRenderManager.h"

void MRI::RenderManager::Init()
{

}
void MRI::RenderManager::PostLoadInit()
{

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

}

// 描画
void MRI::RenderManager::Draw() const
{
	// 光を遮るオブジェクト(不透明な物体や"2D"キャラ)は"Begin"と"End"の間にまとめて"Draw"する
	KdShaderManager::Instance().m_StandardShader.BeginGenerateDepthMapFromLight();
	{

	}
	KdShaderManager::Instance().m_StandardShader.EndGenerateDepthMapFromLight();

	// 陰影のないオブジェクト(背景など)は"Begin"と"End"の間にまとめて"Draw"する
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
	
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	// 陰影のあるオブジェクト(不透明な物体や"2D"キャラ)は"Begin"と"End"の間にまとめて"Draw"する
	KdShaderManager::Instance().m_StandardShader.BeginLit();
	{
	}
	KdShaderManager::Instance().m_StandardShader.EndLit();

	// 陰影のないオブジェクト(エフェクトなど)は"Begin"と"End"の間にまとめて"Draw"する
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)は"Begin"と"End"の間にまとめて"Draw"する
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
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
	// "2D"の描画はこの間で行う
	KdShaderManager::Instance().m_spriteShader.Begin();
	{

	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void MRI::RenderManager::DrawDebug() const
{
	// デバッグ情報の描画はこの間で行う
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();
}

// アプリケーション描画終了 
void MRI::RenderManager::EndDraw() const
{
	// "BackBuffer" "->" 画面表示
	KdDirect3D::Instance().WorkSwapChain()->Present(0, 0);
}