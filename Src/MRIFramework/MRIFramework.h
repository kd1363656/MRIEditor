#pragma once

// 定義、定数
#include "Definition/Constant/Editor/MRIEditorCommonConstant.h"
#include "Definition/Constant/MRICommonConstant.h"
#include "Definition/Struct/MRICommonStruct.h"

// シングルトンを安全に扱うための基底クラス
#include "Singleton/MRISingletonBase.h"

// クラスの型ごとにユニークな"ID"を付与するクラス(ダウンキャストに使用)
#include "StaticID/MRIStaticID.h"

// ウィンドウクラス
#include "Window/MRIWindow.h"

// "RenderManager"
#include "Render/MRIRenderManager.h"

// エディター

// "Popup"
#include "Editor/Popup/MRIEditorPopup.h"

// "Menu"
#include "Editor/Menu/MRIEditorMenuBase.h"
#include "Editor/Menu/File/MRIFileMenuEditor.h"
#include "Editor/MenuBar/MRIEditorMenuBar.h"

// "View"
#include "Editor/View/MRIEditorViewBase.h"
#include "Editor/View/Log/MRIEditorLogView.h"

// "Manager"
#include "Editor/MRIEditorManager.h"

// ファイル読み取り便利関数
#include "Utility/FileIO/MRIFileIOUtility.h"

// "InputManager"
#include "Input/MRIInputManager.h"

// "FPS"管理クラス
#include "FPS/MRIFPSController.h"