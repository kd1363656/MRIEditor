#pragma once

// 定義、定数
#include "Definition/Constant/Editor/MRIEditorCommonConstant.h"
#include "Definition/Constant/MRICommonConstant.h"
#include "Definition/Struct/MRICommonStruct.h"

// 継承しているかどうかを確認するコンセプト
#include "Concept/IsDerived/MRIIsDerivedBaseConcept.h"

// シングルトンを安全に扱うための基底クラス
#include "Singleton/MRISingletonBase.h"

// クラスの型ごとにユニークな"ID"を付与するクラス(ダウンキャストに使用)
#include "StaticID/MRIStaticID.h"

// クラスの型情報
#include "TypeInfo/MRITypeInfo.h"

// "型情報便利関数"
#include "Utility/TypeInfo/MRITypeInfoUtility.h"

// 型情報保持クラス
#include "TypeInfo/Registry/MRITypeInfoRegistry.h"

// ウィンドウクラス
#include "Window/MRIWindow.h"

// エルミート補完などの補完クラス
#include "Modifier/Interpolator/MRIInterpolatorModifierBase.h"
#include "Modifier/Interpolator/Hermite/MRIHermiteInterpolatorModifier.h"

// 補完クラスのコンセプト
#include "Concept/IsDerived/MRIIsDerivedInterpolatorModifierBaseConcept.h"

// タグ
#include "Tag/MRITagBase.h"
#include "Tag/Bool/MRIBoolTag.h"
#include "Tag/Event/MRIEventLaneTag.h"
#include "Tag/Event/Input/MRIInputEventTag.h"
#include "Tag/Event/Component/MRIComponentEventTag.h"
#include "Tag/Event/SceneShift/MRISceneShiftEventTag.h"
#include "Tag/Camera/MRICameraTag.h"
#include "Tag/Shader/MRIShaderTag.h"
#include "Tag/Bone/MRIBoneTypeTag.h"
#include "Tag/Axis/MRIAxisTag.h"

// タグの基底構造体を継承しているかどうかを確認する"Concept"
#include "Concept/IsDerived/MRIIsDerivedTagBaseConcept.h"
#include "Concept/IsDerived/MRIIsDerivedAxisTagBaseConcept.h"
#include "Concept/IsDerived/MRIIsDerivedInputEventTagBaseConcept.h"

// "UUID"用便利関数
#include "Utility/UUID/MRIUUIDUtility.h"

// ファイル読み取り便利関数
#include "Utility/FileIO/MRIFileIOUtility.h"

// "FPS"管理クラス
#include "FPS/MRIFPSController.h"

// タグ用便利関数
#include "Utility/Tag/MRITagUtility.h"

// "TypeTrait(Pointer)"
#include "TypeTrait/PTR/MRITypeTraitPTR.h"

// ポインタかどうかを判別するコンセプト
#include "Concept/IsSmartPTR/MRIIsSmartPTRConcept.h"

// "Editor"用便利関数
#include "Utility/Editor/MRIEditorUtility.h"

// "Json"用便利関数
#include "Utility/Json/MRIJsonUtility.h"

// オブザーバーの通知を受け取るクラス
#include "Observer/MRIObserver.h"

// オブザーバー通知クラス
#include "Observer/Notifier/MRIObserverNotifierBase.h"
#include "Observer/Notifier/Keep/MRIObserverNotifierKeep.h"
#include "Observer/Notifier/Moment/MRIObserverNotifierMoment.h"

// "GameObject"用便利関数
#include "Utility/GameObject/MRIGameObjectUtility.h"

// 仮想キーコード用便利関数
#include "Utility/VKCode/MRIVKCodeUtility.h"

// エディター
#include "Editor/Popup/MRIEditorPopup.h"
#include "Editor/Menu/MRIEditorMenuBase.h"
#include "Editor/Menu/File/MRIFileMenuEditor.h"
#include "Editor/Menu/MRIEditorMenuBar.h"
#include "Editor/View/MRIEditorViewBase.h"
#include "Editor/View/Log/MRIEditorLogView.h"
#include "Editor/View/Game/MRIEditorGameView.h"
#include "Editor/View/Hierarchy/MRIEditorHierarchyView.h"
#include "Editor/View/Inspector/PlayerKeyConfig/MRIEditorPlayerKeyConfigInspectorView.h"
#include "Editor/View/Inspector/Prefab/GameObject/MRIEditorPrefabGameObjectInspectorView.h"
#include "Editor/View/Inspector/Prefab/Registry/SceneShift/MRIEditorPrefabSceneShiftRegistryInspectorView.h"
#include "Editor/View/Inspector/Prefab/Registry/GameObject/MRIEditorPrefabGameObjectRegistryInspectorView.h"
#include "Editor/View/Inspector/Spawn/GameObject/MRIEditorSpawnGameObjectInspectorView.h"
#include "Editor/View/Inspector/Spawn/SceneLoad/Prefab/MRIEditorSpawnSceneLoadPrefabInspectorView.h"
#include "Editor/View/Inspector/Spawn/SceneLoad/SceneShift/MRIEditorSpawnSceneLoadSceneShiftInspectorView.h"

// "Manager"
#include "Editor/MRIEditorManager.h"

// ファクトリークラス
#include "Factory/MRIGenericFactory.h"

// "Scene"
#include "Scene/MRIScene.h"
#include "Scene/MRISceneManager.h"

// 計算便利関数
#include "Utility/Math/MRIMathUtility.h"

// "InputManager"
#include "Input/MRIInputManager.h"

// キーコンフィグクラス
#include "PlayerKeyConfig/MRIPlayerKeyConfig.h"

// "Axis"用便利関数
#include "Utility/Axis/MRIAxisUtility.h"

// コンポーネントの基底クラス
#include "Component/MRIComponentBase.h"
// "TransformComponent"
#include "Component/Transform/MRITransformComponent.h"

// コンポーネントの基底クラスを継承しているかどうかを確認する"Concept"
#include "Concept/IsDerived/MRIIsDerivedComponentBaseConcept.h"

// ヘルパークラス
#include "Helper/AssetFilePath/MRIAssetFilePathHelper.h"
#include "Helper/Component/MRIComponentHelperBase.h"
#include "Helper/Component/Rotation/ByMouse/MRIComponentRotationByMouseHelper.h"
#include "Helper/Component/UUIDFetcher/MRIComponentUUIDFetcherHelper.h"

// コンポーネントモードクラス
// コンポーネントの挙動を切り替えたり必要な変数を保持、必要な処理をまとめたクラス
#include "ComponentMode/MRIComponentModeBase.h"

// 回転
#include "ComponentMode/Rotation/MRIRotationComponentModeBase.h"
#include "ComponentMode/Rotation/ConstantSpeed/MRIRotationComponentConstantSpeedModeBase.h"
#include "ComponentMode/Rotation/ConstantSpeed/Mouse/MRIRotationComponentConstantSpeedMouseMode.h"
#include "ComponentMode/Rotation/Lerp/MRIRotationComponentLerpModeBase.h"
#include "ComponentMode/Rotation/Lerp/Mouse/MRIRotationComponentLerpMouseMode.h"
#include "ComponentMode/Rotation/Lerp/TargetViewDirection/MRIRotationComponentLerpTargetViewDirectionMode.h"

// 移動
#include "ComponentMode/Move/MRIMoveComponentModeBase.h"
#include "ComponentMode/Move/ConstantSpeed/MRIMoveComponentConstantSpeedModeBase.h"
#include "ComponentMode/Move/ConstantSpeed/FacingDirection/MRIMoveComponentConstantSpeedFacingDirectionMode.h"

// 入力
#include "ComponentMode/Input/MRIInputComponentModeBase.h"
#include "ComponentMode/Input/Move/MRIInputComponentMoveModeBase.h"
#include "ComponentMode/Input/Move/FourWay/MRIInputComponentMoveFourWayMode.h"
#include "ComponentMode/Input/Rotation/MRIInputComponentRotationModeBase.h"
#include "ComponentMode/Input/Rotation/FourWay/MRIInputComponentRotationFourWayMode.h"

// "Component"
#include "Component/Camera/MRICameraComponent.h"
#include "Component/Render/Model/MRIRenderModelComponentBase.h"
#include "Component/Render/Model/Animation/MRIRenderModelAnimationComponent.h"
#include "Component/Render/Model/Static/MRIRenderModelStaticComponent.h"
#include "Component/Rotation/MRIRotationComponent.h"
#include "Component/Move/MRIMoveComponent.h"
#include "Component/Input/MRIInputComponent.h"

// "Strategy"
#include "Strategy/MRIStrategyBase.h"
#include "Component/Transform/MatrixStrategy/MRIMatrixStrategyBase.h"
#include "Component/Transform/MatrixStrategy/CreateSRT/MRIMatrixStrategyCreateSRT.h"
#include "Component/Transform/MatrixStrategy/CreateSTRParentT/MRIMatrixStrategyCreateSTRParentT.h"

// ファクトリーの呼び出しを間違えないための"using"
#include "Factory/Unique/MRIUniqueFactory.h"
#include "Factory/Shared/MRISharedFactory.h"

// ファクトリー用便利関数
#include "Utility/Factory/MRIFactoryUtility.h"

// コンバーターの基底クラス
#include "Converter/MRIIConverter.h"

// リソース管理クラス
#include "Resource/Prefab/MRIPrefab.h"
#include "Resource/Prefab/MRIPrefabController.h"
#include "Resource/MRIResourceController.h"

// コンバーター
#include "Converter/GameObject/MRIGameObjectConverter.h"
#include "Converter/PrefabController/MRIPrefabControllerConverter.h"
#include "Converter/Scene/MRISceneConverter.h"
#include "Converter/Scene/MRISceneManagerConverter.h"

// "GameObject"
#include "GameObject/MRIGameObject.h"

// ファイル入出力クラス
#include "FileIO/MRIFileIOBase.h"
#include "FileIO/SceneManager/MRISceneManagerFileIO.h"
#include "FileIO/PrefabController/MRIPrefabControllerFileIO.h"

// "RenderManager"
#include "Render/MRIRenderManager.h"

// エディターとシーンの記録を保存する便利関数
#include "Utility/Editor/SceneSave/MRIEditorSceneSaveUtility.h"