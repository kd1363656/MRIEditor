#include "MRIGameObjectUtility.h"

#include "../../GameObject/MRIGameObject.h"

std::shared_ptr<MRI::GameObject> MRI::GameObjectUtility::CreateGameObject()
{
	return std::make_shared<MRI::GameObject>();
}

void MRI::GameObjectUtility::RecursiveAddComponent(std::weak_ptr<MRI::GameObject> a_self , const std::vector<std::shared_ptr<MRI::Component::ComponentBase>>& a_componentList , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList)
{
	auto l_self = a_self.lock();
	if (!l_self) { return; }

	// 親のコンポーネントを親のコンポーネントリストに追加
	for (const auto& l_component : a_componentList)
	{
		if (!l_component) { continue; }
		l_self->AddComponent(l_component);
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
	// 子であろうが一つにリストに格納
	auto l_sceneCache = SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache || !a_parent)
	{
		return;
	}

	// 親子関係を再帰的に登録
	for (auto& l_childLoad : a_childLoadList)
	{
		if (!l_childLoad.self) { continue; }

		a_parent->AttachChildCache (l_childLoad.self);
		l_sceneCache->AddGameObject(l_childLoad.self);
		RecursiveAddChild		   (l_childLoad.self , l_childLoad.childLoadList);
	}
}