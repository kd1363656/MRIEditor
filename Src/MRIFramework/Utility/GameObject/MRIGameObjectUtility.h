#pragma once

namespace MRI
{
	class GameObject;
}

namespace MRI::GameObjectUtility
{
	std::shared_ptr<GameObject> CreateGameObject();

	void RecursiveAddComponent(std::weak_ptr<MRI::GameObject> a_self , const std::vector<std::shared_ptr<MRI::Component::ComponentBase>>& a_addComponentList , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList);

	void RecursiveAddChild(const std::shared_ptr<MRI::GameObject>& a_parent , std::vector<MRI::CommonStruct::ChildLoad> a_childLoadList);

	bool IsPrefabGameObject(const std::shared_ptr<MRI::GameObject>& a_gameObject);
}