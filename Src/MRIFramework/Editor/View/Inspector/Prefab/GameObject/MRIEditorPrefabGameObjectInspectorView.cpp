#include "MRIEditorPrefabGameObjectInspectorView.h"

void MRI::Editor::EditorPrefabGameObjectInspectorView::DrawEditor()
{
	// ヒエラルキーで選ばれているゲームオブジェクトのプレハブ名から
	// プレハブのゲームオブジェクトのキャッシュポインタを取得
	auto l_prefabGameObjectCache = GetSelectedPrefabGameObjectCache().lock();
	if (!l_prefabGameObjectCache) { return; }

	if (!ImGui::Begin("PrefabGameObjectInspector"))
	{
		ImGui::End();
		return;
	}

	DrawAddComponentSelector     ();
	DrawAddComponentButton       (l_prefabGameObjectCache);
	DrawPrefabGameObjectInspector(l_prefabGameObjectCache);

	ImGui::End();
}

void MRI::Editor::EditorPrefabGameObjectInspectorView::DrawAddComponentSelector()
{
	ImGui::SetNextItemWidth(MRI::EditorCommonConstant::k_comboWidth);
	ImGui::BeginGroup      ();

	if (!ImGui::BeginCombo("Add ComponentSelector" , m_addComponentName.c_str()))
	{
		ImGui::EndGroup();
		return;
	}

	// コンポーネントファクトリーの名前から生成したいコンポーネントを選択
	const auto& l_componentFactory = MRI::SharedFactory::Component::GetInstance();
	for (const auto& [l_key , l_value] : l_componentFactory.GetFactoryMap())
	{
		ImGui::PushID(&l_value);

		bool l_isSelected = m_addComponentName == l_key;

		// セレクトされたコンポーネントの名前を格納
		if (!ImGui::RadioButton(l_key.c_str() , l_isSelected))
		{
			ImGui::PopID();
			continue;
		}

		// 選択された項目にカーソルを当てる
		if (l_isSelected)
		{
			ImGui::SetItemDefaultFocus();
		}

		m_addComponentName = l_key;

		ImGui::PopID();
	}

	ImGui::EndCombo();
	ImGui::EndGroup();
}
void MRI::Editor::EditorPrefabGameObjectInspectorView::DrawAddComponentButton(std::weak_ptr<MRI::GameObject> a_prefabGameObjectCache) const
{
	auto l_prefabGameObjectCache = a_prefabGameObjectCache.lock();
	if (!l_prefabGameObjectCache) { return; }

	// ボタンが押されたらコンポーネントを生成
	if (!MRI::EditorUtility::SameLineButton("Add Component")) { return; }

	// コンポーネントファクトリーの名前から名前に対応したコンポーネントを生成
	const auto& l_componentFactory = MRI::SharedFactory::Component::GetInstance();
	auto		l_component		   = l_componentFactory.Create                 (m_addComponentName);
	
	// コンポーネントに所有者を格納して初期化しコンポネントリストに格納
	l_component->SetOwnerCache           (l_prefabGameObjectCache);
	l_component->Init                    ();
	l_prefabGameObjectCache->AddComponent(l_component);
	
	// 変更を反映するためキャッシュの接続を全コンポーネント再度行う
	l_prefabGameObjectCache->PostLoadInit();
}
void MRI::Editor::EditorPrefabGameObjectInspectorView::DrawPrefabGameObjectInspector(std::weak_ptr<MRI::GameObject> a_prefabGameObjectCache) const
{
	auto l_prefabGameObjectCache = a_prefabGameObjectCache.lock();
	if (!l_prefabGameObjectCache) { return; }

	// "TransformComponent"のプレハブパラメータの編集
	if (auto l_selfTransformComponentCache = l_prefabGameObjectCache->GetSelfTransformComponentCache().lock())
	{
		if (ImGui::TreeNode(l_selfTransformComponentCache->GetTypeInfo().k_name.data()))
		{
			l_selfTransformComponentCache->EditPrefabInspector();
			ImGui::TreePop									  ();
		}
	}
	
	// 各種コンポーネントのパラメータを編集する"UI"の描画
	auto& l_componentList = l_prefabGameObjectCache->GetWorkComponentList();
	auto  l_itr           = l_componentList.begin						 ();
	while (l_itr != l_componentList.end())
	{
		auto l_component = *l_itr;
		
		// ヌルだった場合要素を削除
		if (!l_component) 
		{
			l_itr = l_componentList.erase(l_itr);
			continue;
		}

		ImGui::PushID(l_component.get());

		// "ImGui::TreeNode"でコンポーネントの名前を描画
		if (ImGui::TreeNode(l_component->GetTypeInfo().k_name.data()))
		{
			// コンポーネントの削除申請
			if (MRI::EditorUtility::SameLineSmallButton("Delete"))
			{
				l_itr = l_componentList.erase(l_itr);
				ImGui::PopID                 ();
				ImGui::TreePop               ();
				continue;
			}

			// チェックボックスでコンポーネントの有効化と無効化を切り替える
			{
				bool l_isEnable = l_component->GetIsEnabled();
				ImGui::Checkbox("IsEnable", &l_isEnable);

				l_isEnable ? l_component->Enable() : l_component->Disable();
			}

			// コンポーネントの順番の入れ替え
			// 現在のインデックスを取得
			// 上へ移動
			if (auto l_index = std::distance(l_componentList.begin() , l_itr);
				MRI::EditorUtility::SameLineSmallButton(U8("▲"))		  &&
				l_index > 0)
			{
				// このイテレータの前のイテレーターを取得
				auto l_prevITR = std::prev(l_itr);

				// 現在のコンポーネントと前回のコンポーネントを交換
				std::iter_swap(l_prevITR , l_itr);

				// 交換したら現在見ているコンポーネントが"l_prevITR"になるので
				// イテレーターを後退させる
				--l_itr;
				ImGui::PopID  ();
				ImGui::TreePop();

				continue;
			}

			// 下へ移動
			if (MRI::EditorUtility::SameLineSmallButton(U8("▼")) &&
				std::next(l_itr) != l_componentList.end())
			{
				// 一つ進めたイテレーターを取得
				auto l_nextITR = std::next(l_itr);

				// 現在のコンポーネントと先のコンポーネントを交換
				std::iter_swap(l_itr , l_nextITR);

				// 現在のコンポーネントが先のコンポーネントと交換されるので
				// イテレーターを進める
				++l_itr;

				ImGui::PopID  ();
				ImGui::TreePop();
				continue;
			}

			// パラメータの編集
			l_component->EditPrefabInspector();
			
			ImGui::TreePop();
		}

		l_itr++;
		ImGui::PopID();
	}
}

std::weak_ptr<MRI::GameObject> MRI::Editor::EditorPrefabGameObjectInspectorView::GetSelectedPrefabGameObjectCache() const
{
	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) 
	{
		return std::weak_ptr<MRI::GameObject>();
	}

	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) 
	{
		return std::weak_ptr<MRI::GameObject>();
	}

	auto& l_editorManager       = MRI::Editor::EditorManager::GetInstance									   ();
	auto  l_editorHierarchyView = l_editorManager.FetchEditorViewCache<MRI::Editor::EditorHierarchyView>().lock();
	
	if (!l_editorHierarchyView) 
	{ 
		return std::weak_ptr<MRI::GameObject>();
	}

	// ヒエラルキービューから今ヒエラルキーで選ばれているゲームオブジェクトを取得
	auto l_gameObjectCache = l_editorHierarchyView->GetSelectedGameObjectCache().lock();
	if (!l_gameObjectCache) 
	{
		return std::weak_ptr<MRI::GameObject>();
	}

	// ヒエラルキーで取得したゲームオブジェクトの名前からプレハブを取得
	auto l_prefabCache = l_prefabControllerCache->FetchPrefabCache(l_gameObjectCache->GetPrefabName().data()).lock();
	if (!l_prefabCache) 
	{
		return std::weak_ptr<MRI::GameObject>();
	}

	// プレハブで管理されているゲームオブジェクトのポインタを取得
	return l_prefabCache->GetGameObjectCache();
}