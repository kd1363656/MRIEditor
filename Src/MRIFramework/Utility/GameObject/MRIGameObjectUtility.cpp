#include "MRIGameObjectUtility.h"

#include "../../GameObject/MRIGameObject.h"

std::shared_ptr<MRI::GameObject> MRI::GameObjectUtility::CreateGameObject()
{
	return std::make_shared<MRI::GameObject>();
}

void MRI::GameObjectUtility::RecursiveAddComponent(const std::shared_ptr<MRI::GameObject>& a_self , const std::vector<std::shared_ptr<MRI::Component::ComponentBase>>& a_addComponentList , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList)
{
	if (!a_self) { return; }

	// 親のコンポーネントを親のコンポーネントリストに追加
	for (const auto& l_component : a_addComponentList)
	{
		if (!l_component) { continue; }
		a_self->AddComponent(l_component);
	}

	// このコンポーネントを子のコンポーネントリストに再帰的に追加
	for (auto& l_childLoad : a_childLoadList)
	{
		if (!l_childLoad.self) { continue; }
		RecursiveAddComponent(l_childLoad.self , l_childLoad.componentLoadList , l_childLoad.childLoadList);
	}
}

void MRI::GameObjectUtility::RecursiveAddChild(const std::shared_ptr<MRI::GameObject>& a_parent , std::vector<MRI::CommonStruct::ChildLoad> a_childLoadList)
{
	// 子であろうが一つのリストに格納
	auto l_sceneCache = SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache || !a_parent)
	{
		return;
	}

	// 親子関係を再帰的に登録
	for (auto& l_childLoad : a_childLoadList)
	{
		if (!l_childLoad.self) { continue; }

		a_parent->CreateParentChildRelationship(l_childLoad.self);
		l_sceneCache->AddGameObject            (l_childLoad.self);
		RecursiveAddChild		               (l_childLoad.self , l_childLoad.childLoadList);
	}
}

bool MRI::GameObjectUtility::IsPrefabGameObject(const std::shared_ptr<MRI::GameObject>& a_gameObject)
{
	if (!a_gameObject) 
	{
		return false; 
	}

	auto l_sceneCache = SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache)
	{
		return false;
	}

	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) 
	{
		return false; 
	}

	auto l_prefabCache = l_prefabControllerCache->FetchPrefabCache(a_gameObject->GetPrefabName()).lock();
	if (!l_prefabCache) 
	{
		return false;
	}

	auto l_prefabGameObject = l_prefabCache->GetGameObjectCache().lock();
	if (!l_prefabGameObject)
	{
		return false;
	}

	return l_prefabGameObject.get() == a_gameObject.get();
}