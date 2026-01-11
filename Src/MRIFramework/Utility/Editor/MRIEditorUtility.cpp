#include "MRIEditorUtility.h"

bool MRI::EditorUtility::IsPrefabGameObjectInSceneGameObjectList(const std::string& a_prefabName)
{
	auto l_sceneCache = SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) 
	{ 
		return false;
	}

	// 消す予定もしくはプレハブ名が一致しなければ一致しない
	return std::ranges::any_of(l_sceneCache->GetGameObjectList() , [&a_prefabName](const auto& a_gameObject) 
	{
		if (!a_gameObject)					               { return false; }
		if (a_gameObject->GetIsDestroyed())                { return false; }
		if (a_gameObject->GetPrefabName() != a_prefabName) { return false; }
	
		return true;
	});
}
bool MRI::EditorUtility::HasSameUUIDGameObjectInSceneGameObjectList(const UUID& a_uuid)
{
	auto l_sceneCache = SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache)
	{
		return false;
	}

	// 引数と同じ"UUID"を持つゲームオブジェクトが存在するかどうかを確認
	return std::ranges::any_of(l_sceneCache->GetGameObjectList() , [&a_uuid](const auto& a_gameObject)
	{
		if (!a_gameObject)                     { return false; }
		if (a_uuid == GUID_NULL)			   { return false; }
		if (a_uuid != a_gameObject->GetUUID()) { return false; }

		return true;
	});
}
void MRI::EditorUtility::DeletePrefabIfAllInstanceRemoved(const std::string& a_prefabName)
{
	auto l_sceneCache = SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }
	
	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) { return; }
	
	// 見つかっていたらこの先の処理は実行しない
	if (bool l_isFind = MRI::EditorUtility::IsPrefabGameObjectInSceneGameObjectList(a_prefabName);
		l_isFind) 
	{
		return;
	}
	
	// 見つからなかった場合プレハブの名前から現在シーンで読み込むプレハブとして
	// 登録されているプレハブを削除
	l_prefabControllerCache->RemovePrefab(a_prefabName);
}
void MRI::EditorUtility::DeleteGameObjectIfPrefabRemoved(const std::string& a_prefabName)
{
	auto l_sceneCache = SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }
	
	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) { return; }

	// 名前で指定されたプレハブが消されたかどうかをチェック
	if (!l_prefabControllerCache->FetchPrefabCache(a_prefabName).expired()) { return; }
	
	// 消されたプレハブと同じ名前のゲームオブジェクトを削除
	for (const auto& l_gameObject : l_sceneCache->GetGameObjectList())
	{
		if (!l_gameObject)					               { continue; }
		if (l_gameObject->GetIsDestroyed())                { continue; }
		if (l_gameObject->GetPrefabName() != a_prefabName) { continue; }
	
		l_gameObject->Destroy();
	}
}

void MRI::EditorUtility::CreateGameObjectAndRegisterScenePrefab(const std::string& a_prefabName , const std::string& a_prefabFilePath)
{
	auto l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) { return; }

	// プレハブとして格納されているかシーンに配置されていたら"return"
	if (MRI::EditorUtility::IsPrefabGameObjectInSceneGameObjectList(a_prefabName)) { return; }
	if (!l_prefabControllerCache->FetchPrefabCache(a_prefabName).expired())		   { return; }

	// 作製したいゲームオブジェクトのプレハブが追加された、つまり
	// 今このシーン上で使いたいゲームオブジェクトである可能性が高いためシーンで管理するプレハブに自動登録し
	// シーンのリストに自動で追加(プレハブ生成初回のみ)
	auto l_prefab = std::make_shared<MRI::Resource::Prefab>();
	l_prefab->SetFilePath								   (a_prefabFilePath);

	l_prefabControllerCache->AddPrefab(a_prefabName , l_prefab);

	// この関数が動くということは初めて登録されたプレハブでシーンで使う
	// 確率が非常に高いのかつプレハブ用のゲームオブジェクトとして
	// シーンに追加する
	auto l_gameObject = std::make_shared<MRI::GameObject>();
	l_gameObject->Init									 ();
	l_gameObject->SetPrefabName							 (a_prefabName);

	UUID l_uuid = GUID_NULL;
	MRI::UUIDUtility::GenerateUUID(l_uuid);
	l_gameObject->SetUUID         (l_uuid);

	// プレハブにゲームオブジェクトをセット
	l_prefab->SetGameObjectCache(l_gameObject);
	
	// ゲームオブジェクトシーンのリストに格納
	l_sceneCache->AddGameObject(l_gameObject);
}