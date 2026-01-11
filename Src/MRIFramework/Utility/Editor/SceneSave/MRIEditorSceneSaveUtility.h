#pragma once

namespace MRI::EditorSceneSaveUtility
{
	// シーン情報と"Editor"の設定を保存
	inline void Save()
	{
		const auto& l_editorManager = MRI::Editor::EditorManager::GetInstance();
		const auto& l_sceneManager  = MRI::SceneManager::GetInstance         ();

		l_editorManager.SaveFile();
		l_sceneManager.SaveFile ();
	}
}