#include "MRIEditorHierarchyView.h"

void MRI::Editor::EditorHierarchyView::Init()
{
	m_draggingGameObjectCache.reset();
	m_selectedGameObjectCache.reset();

	m_selectedGameObjectUUID = GUID_NULL;

	m_createGameObjectName = MRI::CommonConstant::k_stringUnknown.data();
}

void MRI::Editor::EditorHierarchyView::DrawEditor()
{
	RunOnceSetPrevGameObjectCache();

	if (!ImGui::Begin("HierarchyView"))
	{
		ImGui::End();
		return;
	}

	// ゲームオブジェクトをセレクターから選び生成
	DrawAddGameObjectSelector();
	DrawAddGameObjectButton  ();

	// 見やすいように線を引く
	ImGui::Separator();

	DrawGameObjectHierarchy();

	// 空白位置にドロップしたら親子関係を解除する
	UnparentIfDroppedInEmptyHierarchyArea();

	// もしマウスの左クリックが離されたかヒエラルキービュー外で
	// ドラッグ情報をリセット
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) || !MRI::EditorUtility::IsMouseInWindowRect())
	{
		m_draggingGameObjectCache.reset();
	}
	
	ImGui::End();
}

void MRI::Editor::EditorHierarchyView::LoadFile()
{
	auto l_rootJson = MRI::FileIOUtility::LoadJsonFile(k_fileIOPath);
	if (l_rootJson.is_null()) { return; }

	UUID l_prevSelectedUUID = MRI::JsonUtility::DeserializeUUID(l_rootJson);
	if (l_prevSelectedUUID == GUID_NULL) { return; }

	m_selectedGameObjectUUID = l_prevSelectedUUID;
}
void MRI::Editor::EditorHierarchyView::SaveFile()
{
	// 選択されているゲームオブジェクトが存在するかどうかを確認
	if (!MRI::EditorUtility::HasSameUUIDGameObjectInSceneGameObjectList(m_selectedGameObjectUUID)) { return; }
	
	// 存在して入れば"UUID"を"json"に保存
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson    (l_rootJson , MRI::JsonUtility::SerializeUUID(m_selectedGameObjectUUID));
	MRI::FileIOUtility::SaveJsonFile(l_rootJson , k_fileIOPath);
}

void MRI::Editor::EditorHierarchyView::SetupMainCamera(const std::shared_ptr<GameObject>& a_gameObject)
{
	if (!a_gameObject) { return; }

	// 自身か子にカメラコンポーネントをセット
	if (auto l_cameraComponent = a_gameObject->GetComponentCache<Component::CameraComponent>().lock())
	{
		l_cameraComponent->SetMainCameraTag();
		RenderManager::GetInstance().SetMainCameraComponentCache(l_cameraComponent);
		return;
	}

	for (auto& l_childCache : a_gameObject->GetChildCacheList())
	{
		auto l_child = l_childCache.lock();
		if (!l_child) { continue; }

		SetupMainCamera(l_child);
	}
}

void MRI::Editor::EditorHierarchyView::RunOnceSetPrevGameObjectCache()
{
	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();

	if (!l_sceneCache || m_isFirstTime || m_selectedGameObjectUUID == GUID_NULL) { return; }

	// 一致する"UUID"を持つなら前回選択されたゲームオブジェクトなので今選択されているゲームオブジェクト
	// とみなしてキャッシュを格納する
	for (const auto& l_gameObject : l_sceneCache->GetGameObjectList())
	{
		if (!l_gameObject) { continue; }
		if (l_gameObject->GetUUID() != m_selectedGameObjectUUID) { continue; }

		m_selectedGameObjectCache = l_gameObject;
	}
}

void MRI::Editor::EditorHierarchyView::SetUUID(const std::shared_ptr<MRI::GameObject> a_gameObject)
{
	if (!a_gameObject) { return; }

	// "UUID"を生成
	UUID l_uuid = GUID_NULL;

	// シーン内にあるゲームオブジェクトと同じ"UUID"を取り続けるか"GUID_NULL"なら抽選し直し続ける
	while (l_uuid == GUID_NULL || MRI::EditorUtility::HasSameUUIDGameObjectInSceneGameObjectList(l_uuid))
	{
		MRI::UUIDUtility::GenerateUUID(l_uuid);
	}

	// 最終的に生成された"UUID"をゲームオブジェクトにセット
	a_gameObject->SetUUID(l_uuid);

	for (const auto& l_childCache : a_gameObject->GetChildCacheList())
	{
		const auto& l_child = l_childCache.lock();
		if (!l_child) { continue; }

		SetUUID(l_child);
	}
}

void MRI::Editor::EditorHierarchyView::DrawAddGameObjectSelector()
{
	auto l_sceneCache = SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) { return; }

	// コンボボックスの幅を設定
	ImGui::SetNextItemWidth(MRI::EditorCommonConstant::k_comboWidth);
	ImGui::BeginGroup      ();

	// もしコンボボックスが開かれていなければ早期リターン
	if (!ImGui::BeginCombo("CreateGameObjectSelector" , m_createGameObjectName.c_str()))
	{
		ImGui::EndGroup();
		return;
	}

	// 現在シーン内でロードするゲームオブジェクトのプレハブ情報を記録しているマップにアクセスし
	// そこから生成するゲームオブジェクトを選ぶ
	for (const auto& [l_key , l_value] : l_prefabControllerCache->GetPrefabMap())
	{
		if (!l_value) { continue; }

		bool l_isSelected = m_createGameObjectName == l_key;

		ImGui::PushID(&l_value);

		if (ImGui::RadioButton(l_key.c_str() , l_isSelected))
		{
			m_createGameObjectName = l_key;
		}

		// 選択された項目にカーソルを当てる
		if (l_isSelected)
		{
			ImGui::SetItemDefaultFocus();
		}

		ImGui::PopID();
	}

	ImGui::EndCombo();
	ImGui::EndGroup();
}
void MRI::Editor::EditorHierarchyView::DrawAddGameObjectButton()
{
	// ボタンがクリックされたらゲームオブジェクト生成処理を実行
	if (!MRI::EditorUtility::SameLineButton("Add GameObject")) { return; }

	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) { return; }

	// ゲームオブジェクトを生成しプレハブ名を格納
	auto l_gameObject = MRI::GameObjectUtility::CreateGameObject();
	l_gameObject->Init											();
	l_gameObject->SetPrefabName									(m_createGameObjectName);

	// プレハブが存在すれば必要なコンポーネントと子ゲームオブジェクトを
	// デシリアライズして作成して格納
	if (auto l_prefabCache = l_prefabControllerCache->FetchPrefabCache(m_createGameObjectName).lock();
		l_prefabCache)
	{
		const auto& l_json = l_prefabCache->GetJson();

		if (!l_json.is_null())
		{
			// デシリアライズ用リストを作成
			std::vector<std::shared_ptr<MRI::Component::ComponentBase>> l_componentList;
			std::vector<MRI::CommonStruct::ChildLoad>                   l_childLoadList;

			// プレハブ情報を読み込み再帰的に子と親を追加する
			l_gameObject->DeserializePrefab              (l_json       , l_componentList , l_childLoadList);
			MRI::GameObjectUtility::RecursiveAddComponent(l_gameObject , l_componentList , l_childLoadList);
			MRI::GameObjectUtility::RecursiveAddChild    (l_gameObject , l_childLoadList);

			// 読み込んだ後にコンポーネント同士の接続やリソースのロードを行う
			l_gameObject->PostLoadInit();

			// シーンのリストに追加
			l_sceneCache->AddGameObject(l_gameObject);

			for (const auto& l_component : l_gameObject->GetComponentList())
			{
				if (!l_component) { continue; }

				// もし"renderModelComponentCache"がうまくキャストできなければ空の"std::weak_ptr<MRI::Component::renderModelComponentBase>"が帰ってくる
				auto l_renderModelComponentCache = MRI::TypeInfoUtility::SafeCast<MRI::Component::RenderModelComponentBase>(l_component);
				if (l_renderModelComponentCache.expired()) { continue; }

				// 描画コンポーネントを描画マネージャーに追加
				MRI::RenderManager::GetInstance().AddRenderModelComponentBase(l_renderModelComponentCache);
			}
		}

		// もしプレハブ反映用ゲームオブジェクトが存在しなければ
		// 今生成したゲームオブジェクトをキャッシュに格納
		SetupPrefabCache(l_gameObject);
		SetUUID		    (l_gameObject);
	}

	// 追加したゲームオブジェクトの"Inspector"を捜査したい可能性が非常に高いため
	// 自動的に生成したゲームオブジェクトを今選択しているゲームオブジェクトとして扱う
	SetSelectedGameObjectCache(l_gameObject);
}
void MRI::Editor::EditorHierarchyView::DrawGameObjectHierarchy()
{
	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	for (const auto& l_gameObject : l_sceneCache->GetGameObjectList())
	{
		// ヌルポインタか親が存在しなければ"continue"
		if (!l_gameObject)             { continue; }
		if (l_gameObject->HasParent()) { continue; }

		RecursiveDrawGameObjectHierarchy(l_gameObject);
	}
}
void MRI::Editor::EditorHierarchyView::RecursiveDrawGameObjectHierarchy(const std::shared_ptr<MRI::GameObject>& a_gameObject)
{
	if (!a_gameObject) { return; }

	ImGui::PushID(a_gameObject.get());

	// 子を持たなければ"Lear"フラグを設定
	ImGuiTreeNodeFlags l_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	
	// 子を持たなければ"Leaf"フラグを設定
	if (a_gameObject->GetChildCacheList().empty()) 
	{
		l_flags |= ImGuiTreeNodeFlags_Leaf;
	}

	const bool l_isPrefabGameObject = MRI::GameObjectUtility::IsPrefabGameObject(a_gameObject);

	// もしプレハブゲームオブジェクトなら色を変える
	if (l_isPrefabGameObject)
	{
		ImGui::PushStyleColor(ImGuiCol_Text , k_prefabGameObjectColor);
	}

	// ノードの描画
	// ノードが開いていなければ"return"
	if (!ImGui::TreeNodeEx(a_gameObject->GetPrefabName().data() , l_flags))
	{
		if (l_isPrefabGameObject)
		{
			ImGui::PopStyleColor();
		}

		ImGui::PopID();
		return; 
	}

	if (l_isPrefabGameObject)
	{
		ImGui::PopStyleColor();
	}

	// クリックしたゲームオブジェクトを選択されているゲームオブジェクトとして格納
	if (ImGui::IsItemClicked())
	{
		SetSelectedGameObjectCache(a_gameObject);

		// 選択したオブジェクトがカメラならカメラの主導権を渡す
		SetupMainCamera(a_gameObject);
	}

	// ドラッグアンドドロップで親子関係を結ぶ処理
	HandleHierarchyDragAndDrop(a_gameObject);

	// コンテキストメニューの表示
	HandleContextMenu(a_gameObject);

	// ノードの再帰的な描画
	for (const auto& l_childCache : a_gameObject->GetChildCacheList())
	{
		auto l_child = l_childCache.lock();
		if (!l_child) { continue; }

		RecursiveDrawGameObjectHierarchy(l_child);
	}

	ImGui::PopID  ();
	ImGui::TreePop();
}

void MRI::Editor::EditorHierarchyView::HandleHierarchyDragAndDrop(const std::weak_ptr<MRI::GameObject> a_gameObjectCache)
{
	auto l_gameObjectCache = a_gameObjectCache.lock();
	if (!l_gameObjectCache) { return; }

	// ドラッグドロップソース
	if (MRI::EditorUtility::DragDropSource("GameObject" , l_gameObjectCache)) 
	{
		// dラッグしているゲームオブジェクトを格納
		m_draggingGameObjectCache = l_gameObjectCache;
	}
	
	// ドラッグドロップターゲット
	std::shared_ptr<MRI::GameObject> l_dropped = nullptr;
	MRI::EditorUtility::DragDropTarget("GameObject" , l_dropped);

	// "DragDropTarget"でポインタを受け取れなかったら"nullptr"なので"return"
	if (!l_dropped) { return; }

	// ドロップしたゲームオブジェクトを格納
	m_draggingGameObjectCache = l_dropped;

	// 子孫なら意味のないドロップになるので"return"
	if (IsDescendant(l_gameObjectCache, l_dropped)) { return; }

	// 既に親を持っているなら親子関係を断ち切る
	if (auto l_parent = l_dropped->GetParentCache().lock())
	{
		MRI_ADD_LOG("親子関係が解除されました\n親 : %s , 子 : %s", l_parent->GetPrefabName().data(), l_dropped->GetPrefabName().data());

		l_parent->RemoveChildCache (l_dropped);
		l_dropped->ResetParentCache();
	}

	// 新たな親子関係を構築
	l_gameObjectCache->CreateParentChildRelationship(l_dropped);

	MRI_ADD_LOG("親子関係が構築されました\n親 : %s , 子 : %s" , l_gameObjectCache->GetPrefabName().data() , l_dropped->GetPrefabName().data());
}

void MRI::Editor::EditorHierarchyView::HandleContextMenu(const std::weak_ptr<MRI::GameObject> a_gameObjectCache) const
{
	auto l_gameObjectCache = a_gameObjectCache.lock();
	if (!l_gameObjectCache) { return; }

	// 右クリックで削除コンテキストメニュー
	if (ImGui::BeginPopupContextItem("GameObjectContext"))
	{
		// クリックされたら削除申請を送る
		if (ImGui::MenuItem(U8("削除")))
		{
			l_gameObjectCache->Destroy();
		}

		ImGui::EndPopup();
	}
}

void MRI::Editor::EditorHierarchyView::UnparentIfDroppedInEmptyHierarchyArea() const
{
	// ドロップ対h層がなければ処理しない
	auto l_droppedCache = m_draggingGameObjectCache.lock();
	if (!l_droppedCache) { return; }

	// マウスがこの領域内にある時のみ実行
	if (!ImGui::IsMouseReleased(ImGuiMouseButton_Left)) { return; }
	if (!MRI::EditorUtility::IsMouseInWindowRect())		{ return; }

	auto l_oldParent = l_droppedCache->GetParentCache().lock();
	if (!l_oldParent) { return; }

	l_oldParent->RemoveChildCache   (l_droppedCache);
	l_droppedCache->ResetParentCache();

	MRI_ADD_LOG("親子関係が解除されました\n親 : %s , 子 : %s" , l_oldParent->GetPrefabName().data(), l_droppedCache->GetPrefabName().data());
}

bool MRI::Editor::EditorHierarchyView::IsDescendant(const std::weak_ptr<MRI::GameObject> a_parentCache , const std::weak_ptr<MRI::GameObject> a_targetCache)
{
	auto l_parentCache = a_parentCache.lock();
	auto l_targetCache = a_targetCache.lock();

	if (!l_parentCache || !l_targetCache) 
	{
		return false; 
	}

	for (const auto& l_childCache : l_parentCache->GetChildCacheList())
	{
		auto l_child = l_childCache.lock();
		if (!l_child) { continue; }

		// 自身か、子が同じポインタを示すなら子孫である
		if (l_child.get() == l_targetCache.get() || IsDescendant(l_childCache , l_targetCache))
		{
			return true;
		}
	}

	return false;
}

void MRI::Editor::EditorHierarchyView::SetSelectedGameObjectCache(const std::weak_ptr<MRI::GameObject> a_gameObjectCache)
{
	auto l_gameObjectCache = a_gameObjectCache.lock();
	if (!l_gameObjectCache) { return; }

	m_selectedGameObjectCache = l_gameObjectCache;
	m_selectedGameObjectUUID  = l_gameObjectCache->GetUUID();
}

void MRI::Editor::EditorHierarchyView::SetupPrefabCache(const std::shared_ptr<MRI::GameObject> a_gameObject)
{
	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) { return; }

	if (auto l_prefabCache = l_prefabControllerCache->FetchPrefabCache(a_gameObject->GetPrefabName()).lock();
		l_prefabCache)
	{
		// もしプレハブ反映用ゲームオブジェクトが存在しなければ
		// 今生成したゲームオブジェクトをキャッシュに格納
		if (l_prefabCache->GetGameObjectCache().expired())
		{
			l_prefabCache->SetGameObjectCache(a_gameObject);
		}

		//for (const auto& l_gameObjectCache : l_sceneCache->GetGameObjectList())
		//{
		//	// 自身は含めない
		//	if (a_gameObject  == l_gameObjectCache) { continue; }
		//	if (auto l_prefab = l_prefabCache->GetGameObjectCache().lock();
		//		l_prefab)
		//	{
		//		
		//	}

		//}

		for (const auto& l_childCache : a_gameObject->GetChildCacheList())
		{
			auto l_child = l_childCache.lock();
			if (!l_child) { continue; }

			SetupPrefabCache(l_child);
		}
	}
}