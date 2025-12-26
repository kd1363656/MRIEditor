#pragma once

// 定義、定数
#include "Definition/Constant/Editor/MRIEditorCommonConstant.h"
#include "Definition/Constant/MRICommonConstant.h"
#include "Definition/Struct/MRICommonStruct.h"

// ファイル読み取り便利関数
#include "Utility/FileIO/MRIFileIOUtility.h"

// シングルトンを安全に扱うための基底クラス
#include "Singleton/MRISingletonBase.h"

// ウィンドウクラス
#include "Window/MRIWindow.h"

// "RenderManager"
#include "Render/MRIRenderManager.h"

// エディター管理クラス
#include "Editor/MRIEditorManager.h"

// "InputManager"
#include "Input/MRIInputManager.h"

// "FPS"管理クラス
#include "FPS/MRIFPSController.h"