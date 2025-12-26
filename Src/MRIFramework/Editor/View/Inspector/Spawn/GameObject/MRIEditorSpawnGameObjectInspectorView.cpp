#include "MRIEditorSpawnGameObjectInspectorView.h"

void MRI::Editor::EditorSpawnGameObjectInspectorView::DrawEditor()
{
	auto& l_editorManager       = MRI::Editor::EditorManager::GetInstance									   ();
	auto  l_editorHierarchyView = l_editorManager.FetchEditorViewCache<MRI::Editor::EditorHierarchyView>().lock();

	if (!l_editorHierarchyView) { return; }

	auto l_gameObjectCache = l_editorHierarchyView->GetSelectedGameObjectCache().lock();
	if (!l_gameObjectCache) { return; }

	if (!ImGui::Begin("SpawnGameObjectInspector"))
	{
		ImGui::End();
		return;
	}

	// "TransformComponent"のプレハブパラメータの編集
	if (auto l_selfTransformComponentCache = l_gameObjectCache->GetSelfTransformComponentCache().lock())
	{
		if (ImGui::TreeNode(l_selfTransformComponentCache->GetTypeInfo().k_name.data()))
		{
			l_selfTransformComponentCache->EditSpawnInspector();
			ImGui::TreePop();
		}
	}

	// コンポーネントの"SpawnInspector"を表示
	for (const auto& l_component : l_gameObjectCache->GetComponentList())
	{
		if (!l_component) { return; }
		
		ImGui::PushID(l_component.get());

		if (ImGui::TreeNode(l_component->GetTypeInfo().k_name.data()))
		{
			l_component->EditSpawnInspector();
			ImGui::TreePop				   ();
		}

		ImGui::PopID();
	}

	ImGui::End();
}