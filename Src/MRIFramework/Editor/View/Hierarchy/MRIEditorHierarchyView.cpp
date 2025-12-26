#include "MRIEditorHierarchyView.h"

void MRI::Editor::EditorHierarchyView::Init()
{
	m_draggingGameObjectCache.reset();
}
void MRI::Editor::EditorHierarchyView::DrawEditor()
{
	RunOnceSetPrevGameObjectCache();

	if (!ImGui::Begin("Hierarchy"))
	{
		ImGui::End();
		return;
	}

	// ゲームオブジェクトを選びボタンで生成する
	DrawAddGameObjectSelector();
	DrawAddGameObjectButton  ();
	
	// 見やすいように線を引く
	ImGui::Separator();

	// 親子関係描画
	DrawHierarchy();

	// もしヒエラルキービュー内かつ一番下の何もない領域でゲームオブジェクトをドロップすると
	// ルートゲームオブジェクトとして扱うようにする
	UnparentIfDroppedInEmptyHierarchyArea();

	// もしマウスの左クリックが離されたかヒエラルキービュー外で
	// ドラッグ情報をリセット
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) || 
		!MRI::EditorUtility::IsMouseInWindowRect())
	{
		m_draggingGameObjectCache.reset();
	}

	ImGui::End();
}

void MRI::Editor::EditorHierarchyView::LoadFile()
{
	auto l_rootJson = MRI::FileIOUtility::LoadJsonFile(k_fileIOPath.data());
	if (l_rootJson.is_null()) { return; }

	UUID l_prevSelectedUUID = MRI::JsonUtility::DeserializeUUID(l_rootJson);
	if (l_prevSelectedUUID == GUID_NULL) { return; }

	m_selectedGameObjectUUID = l_prevSelectedUUID;
}
void MRI::Editor::EditorHierarchyView::SaveFile()
{
	// 選択されているゲームオブジェクトが存在するかどうかを確認
	if (!IsExistSelectedGameObject()) { return; }
	
	// 存在して入れば"json"を保存
	auto l_rootJson = nlohmann::json();
	MRI::JsonUtility::UpdateJson    (l_rootJson , MRI::JsonUtility::SerializeUUID(m_selectedGameObjectUUID));
	MRI::FileIOUtility::SaveJsonFile(l_rootJson , k_fileIOPath.data				 ());
}

void MRI::Editor::EditorHierarchyView::RunOnceSetPrevGameObjectCache()
{
	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();

	if (!l_sceneCache                      || 
		m_isFirstTime                      ||
		m_selectedGameObjectUUID == GUID_NULL) { return; }
	
	// 一致する"UUID"を持つなら前回選択されたゲームオブジェクトなので今選択されているゲームオブジェクト
	// とみなしてキャッシュを格納する
	for (const auto& l_gameObject : l_sceneCache->GetGameObjectList())
	{
		if (!l_gameObject)										 { continue; }
		if (l_gameObject->GetUUID() != m_selectedGameObjectUUID) { continue; }

		m_selectedGameObjectCache = l_gameObject;
	}
}

void MRI::Editor::EditorHierarchyView::DrawAddGameObjectSelector()
{
	auto l_sceneCache = SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) { return; }

	ImGui::SetNextItemWidth(MRI::EditorCommonConstant::k_comboWidth);
	ImGui::BeginGroup      ();

	// もしコンボボックスが開かれていなければ早期リターン
	if (!ImGui::BeginCombo("GameObjectTypeSelector" , m_createGameObjectName.c_str()))
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

	// "UUID"を生成
	UUID l_uuid = GUID_NULL;
	MRI::UUIDUtility::GenerateUUID(l_uuid);
	l_gameObject->SetUUID         (l_uuid);

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

			// もしプレハブ反映用ゲームオブジェクトが存在しなければ
			// 今生成したゲームオブジェクトをきゃしゅに格納
			if (l_prefabCache->GetGameObjectCache().expired())
			{
				l_prefabCache->SetGameObjectCache(l_gameObject);
			}
		}
	}

	// 追加したゲームオブジェクトの"Inspector"を捜査したい可能性が非常に高いため
	// 自動的に生成したゲームオブジェクトを今選択しているゲームオブジェクトとして扱う
	SetSelectedGameObject(l_gameObject , l_gameObject->GetUUID());
}
void MRI::Editor::EditorHierarchyView::DrawHierarchy()
{
	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	for (const auto& l_gameObject : l_sceneCache->GetWorkGameObjectList())
	{
		if (!l_gameObject) { continue; }
		// シーンにあるゲームオブジェクトリストは全てのゲームオブジェクトの実態が保存されているが
		// 親がいるならヒエラルキー内に描画しない、それは親の再帰的処理中に描画されるべきものだから
		if (l_gameObject->HasParent()) { continue; }
		RecursiveDrawHierarchy(l_gameObject);
	}
}
void MRI::Editor::EditorHierarchyView::UnparentIfDroppedInEmptyHierarchyArea() const
{
	// ドロップ対h層がなければ処理しない
	auto l_droppedCache = m_draggingGameObjectCache.lock();
	if (!l_droppedCache) { return; }

	// マウスがこの領域内にある時のみ実行
	if (!ImGui::IsMouseReleased                 (ImGuiMouseButton_Left)) { return; }
	if (!MRI::EditorUtility::IsMouseInWindowRect())						 { return; }

	auto l_oldParent = l_droppedCache->GetParentCache().lock();
	if (!l_oldParent) { return; }

	l_oldParent->RemoveChildCache   (l_droppedCache);
	l_droppedCache->ResetParentCache();
}

void MRI::Editor::EditorHierarchyView::RecursiveDrawHierarchy(std::weak_ptr<MRI::GameObject> a_gameObjectCache)
{
	auto l_parentCache = a_gameObjectCache.lock();
	if (!l_parentCache) { return; }

	ImGui::PushID(std::to_address(l_parentCache));
	DrawHierarchy(l_parentCache);
	ImGui::PopID ();
}

void MRI::Editor::EditorHierarchyView::DrawHierarchyContextMenu(std::weak_ptr<MRI::GameObject> a_gameObjectCache) const
{
	auto l_parentCache = a_gameObjectCache.lock();
	if (!l_parentCache) { return; }

	// コンテキストメニューを表示
	if (ImGui::BeginPopupContextItem(l_parentCache->GetPrefabName().data()))
	{
		if (ImGui::MenuItem("Delete"))
		{
			l_parentCache->Destroy();

			// プレハブの名前からこのシーンから存在しなくなったプレハブを探し出し削除
			const std::string l_prefabName = l_parentCache->GetPrefabName().data();
			MRI::EditorUtility::DeletePrefabIfAllInstanceRemoved         (l_prefabName);
			MRI::EditorUtility::DeleteGameObjectIfPrefabRemoved          (l_prefabName);
		}
		ImGui::EndPopup();
	}
}
void MRI::Editor::EditorHierarchyView::DrawHierarchy(std::weak_ptr<MRI::GameObject> a_gameObjectCache)
{
	auto l_parentCache = a_gameObjectCache.lock();
	if (!l_parentCache) { return; }

	const bool l_hasChild = !l_parentCache->GetChildCacheList().empty();

	// ツリーフラグ設定
	ImGuiTreeNodeFlags l_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	// 子を持たなければリーフノードとして描画
	if (!l_hasChild)
	{
		l_flags |= ImGuiTreeNodeFlags_Leaf			 | 
				   ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	const bool l_isOpen = ImGui::TreeNodeEx(l_parentCache->GetPrefabName().data() , l_flags);

	// もし該当するゲームオブジェクトがクリックされていたら
	// "SpawnData"もしくは"PrefabData"を操作するためのインスペクター用ポインタをキャッシュ
	if (ImGui::IsItemClicked())
	{
		SetSelectedGameObject(l_parentCache , l_parentCache->GetUUID());
	}

	HandleHierarchyDragAndDrop(l_parentCache);
	DrawHierarchyContextMenu  (l_parentCache);

	if (l_isOpen && l_hasChild)
	{
		for (const auto& l_childCache : l_parentCache->GetChildCacheList())
		{
			auto l_child = l_childCache.lock();
			if (!l_child) { continue; }

			RecursiveDrawHierarchy(l_child);
		}
		ImGui::TreePop();
	}
}

void MRI::Editor::EditorHierarchyView::HandleHierarchyDragAndDrop(std::weak_ptr<MRI::GameObject> a_gameObjectCache)
{
	auto l_parentCache = a_gameObjectCache.lock();
	if (!l_parentCache) { return; }

	const std::string l_prefabName = l_parentCache->GetPrefabName().data();

	// 親がいなければルートオブジェクトなので格納する必要がない
	if (const bool l_isDragging = MRI::EditorUtility::DragDropSource(l_prefabName.c_str() , l_parentCache);
		l_isDragging && l_parentCache->HasParent())
	{
		// 今ドラッグしているゲームオブジェクトととして格納
		m_draggingGameObjectCache = l_parentCache;
	}

	std::shared_ptr<MRI::GameObject> l_dropped = nullptr;
	MRI::EditorUtility::DragDropTarget(l_prefabName.c_str() , l_dropped);

	// ヌルポインターがドロップ操作に失敗しているかを判別
	if (!l_dropped) 
	{
		return;
	}
	
	// ドロップした対象が同じゲームオブジェクトなら"return"
	if (l_dropped == l_parentCache) { return; }

	// "dropped"のドロップ先が"l_parent"の子孫なら"return"
	if (IsDescendantOf(l_dropped , l_parentCache)) { return; }

	// ドロップが成功したらドロップされる前の自身の親との親子関係を切り離す
	if (auto l_oldParentCache = l_dropped->GetParentCache().lock())
	{
		m_draggingGameObjectCache = l_dropped;

		l_oldParentCache->RemoveChildCache(l_dropped);
		l_dropped->ResetParentCache       ();
	}

	// 親子関係の再設定
	l_parentCache->AttachChildCache(l_dropped);
}

bool MRI::Editor::EditorHierarchyView::IsDescendantOf(std::weak_ptr<MRI::GameObject> a_descendantCache, std::weak_ptr<MRI::GameObject> a_ancestorCache)
{
	auto l_descendantCache = a_descendantCache.lock();
	auto l_ancestorCache   = a_ancestorCache.lock  ();

	if (!l_descendantCache || !l_ancestorCache) { return false; }

	// 祖先かどうかを再帰的に調べる
	for (const auto& l_childCache : l_descendantCache->GetChildCacheList())
	{
		auto l_child = l_childCache.lock();
		if (!l_child) { return false; }
	
		if (l_child == l_ancestorCache)				  { return true; }
		if (IsDescendantOf(l_child, a_ancestorCache)) { return true; }
	}

	return false;
}

bool MRI::Editor::EditorHierarchyView::IsExistSelectedGameObject()
{
	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return false; }

	// 一致する"UUID"を持つゲームオブジェクトを見つけた場合にのみ実行
	UUID l_uuid = m_selectedGameObjectUUID;
	return std::ranges::any_of(l_sceneCache->GetGameObjectList() , [&l_uuid](const auto& l_gameObject)
	{
		if (!l_gameObject)					   { return false; }
		if (l_uuid == GUID_NULL)			   { return false; }
		if (l_uuid != l_gameObject->GetUUID()) { return false; }

		return true;
	});
}

void MRI::Editor::EditorHierarchyView::SetSelectedGameObject(const std::shared_ptr<MRI::GameObject> a_gameObjectCache , const UUID& a_uuid)
{
	if (!a_gameObjectCache) { return; }
	m_selectedGameObjectCache = a_gameObjectCache;
	m_selectedGameObjectUUID  = a_uuid;
}