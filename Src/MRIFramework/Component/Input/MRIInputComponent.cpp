#include "MRIInputComponent.h"

#include "Application/main.h"

const MRI::TypeInfo& MRI::Component::InputComponent::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::InputComponent>();
}

void MRI::Component::InputComponent::Init()
{
	m_inputComponentModeList.clear();
}
void MRI::Component::InputComponent::PostLoadInit()
{
	auto l_ownerCache = MRI::Component::ComponentBase::GetWorkOwnerCache().lock();
	if (!l_ownerCache) { return; }

	for (const auto& l_inputComponentMode : m_inputComponentModeList)
	{
		if (!l_inputComponentMode) { continue; }
		l_inputComponentMode->PostLoadInit(l_ownerCache);
	}
}

void MRI::Component::InputComponent::EarlyUpdate()
{	
	for (const auto& l_inputComponentMode : m_inputComponentModeList)
	{
		if (!l_inputComponentMode)    { continue; }
		if (ImGui::IsAnyItemActive()) { continue; }	// "ImGui"走査中なら"continue"
		l_inputComponentMode->EarlyUpdate();
	}
}

void MRI::Component::InputComponent::EditPrefabInspector()
{
	EditAddInputComponentModeList();

	auto l_itr = m_inputComponentModeList.begin();

	while (l_itr != m_inputComponentModeList.end())
	{
		const auto& l_inputComponentMode = *(l_itr);

		// もしからのインスタンスなら削除
		if (!l_inputComponentMode)
		{
			l_itr = m_inputComponentModeList.erase(l_itr);
			continue;
		}

		ImGui::PushID(std::to_address(l_itr));

		// ノード描画が成功しなければ"continue"
		if (!ImGui::TreeNode("Mode : %s" , l_inputComponentMode->GetTypeInfo().k_name.data()))
		{
			l_itr++;
			ImGui::PopID();
			continue;
		}

		// 順序入れ替えボタン
		if (auto l_index = std::distance(m_inputComponentModeList.begin() , l_itr);
			MRI::EditorUtility::SameLineSmallButton(U8("▲")) && l_index > 0)
		{
			// このイテレータの前のイテレーターを取得
			auto l_prevITR = std::prev(l_itr);

			// 現在の"InputComponentMode"と前回の"InputComponentMode"を交換
			std::iter_swap(l_prevITR , l_itr);

			// イテレーターを後退させる
			--l_itr;
			ImGui::PopID  ();
			ImGui::TreePop();

			continue;
		}

		// 下へ移動
		if (MRI::EditorUtility::SameLineSmallButton(U8("▼")) && std::next(l_itr) != m_inputComponentModeList.end())
		{
			// 一つ進めたイテレーターを取得
			auto l_nextITR = std::next(l_itr);

			// 現在の"InputComponentMode"と前回の"InputComponentMode"を交換
			std::iter_swap(l_itr, l_nextITR);

			// イテレーターを進める
			++l_itr;

			ImGui::PopID  ();
			ImGui::TreePop();
			continue;
		}

		// 削除処理
		if (MRI::EditorUtility::SameLineSmallButton("Delete"))
		{
			l_itr = m_inputComponentModeList.erase(l_itr);
			ImGui::PopID  ();
			ImGui::TreePop();
			continue;
		}

		// インスペクターの描画
		l_inputComponentMode->EditPrefabInspector();

		ImGui::TreePop();
		ImGui::PopID  ();

		l_itr++;
	}
}

void MRI::Component::InputComponent::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null())											   { return; }
	if (!MRI::JsonUtility::IsArray(a_json , "InputComponentModeList")) { return; }

	for (const auto& l_json : a_json["InputComponentModeList"])
	{
		// ファクトリーで生成したクラスのインスタンスが存在すればリストに格納
		std::shared_ptr<MRI::ComponentMode::InputComponentModeBase> l_inputComponentMode = nullptr;
		MRI::JsonUtility::DeserializeInstancePrefab<MRI::SharedFactory::InputComponentMode>(l_json , "InputComponentModeName" , l_inputComponentMode);

		if (l_inputComponentMode)
		{
			m_inputComponentModeList.emplace_back(l_inputComponentMode);
		}
	}
}

nlohmann::json MRI::Component::InputComponent::SerializePrefab()
{
	auto l_rootJson  = nlohmann::json       ();
	auto l_jsonArray = nlohmann::json::array();

	for (const auto& l_inputComponentMode : m_inputComponentModeList)
	{
		if (!l_inputComponentMode) { continue; }

		auto l_json = nlohmann::json();

		MRI::JsonUtility::UpdateJson(l_json , MRI::JsonUtility::SerializeInstancePrefab("InputComponentModeName" , l_inputComponentMode));

		l_jsonArray.emplace_back(l_json);
	}

	l_rootJson["InputComponentModeList"] = l_jsonArray;

	return l_rootJson;
}

bool MRI::Component::InputComponent::IsAlreadyContains(const std::shared_ptr<MRI::ComponentMode::InputComponentModeBase> a_addComponentMode)
{
	if (!a_addComponentMode)
	{
		return false; 
	}

	// 同じものを再度登録しないようにする
	return std::ranges::any_of(m_inputComponentModeList , [&a_addComponentMode](const auto& a_inputComponentMode) -> bool
	{
		if (!a_inputComponentMode)
		{
			return false; 
		}

		return a_addComponentMode->GetTypeInfo().k_id == a_inputComponentMode->GetTypeInfo().k_id;
	});
}

void MRI::Component::InputComponent::EditAddInputComponentModeList()
{
	auto l_ownerCache = MRI::Component::ComponentBase::GetWorkOwnerCache().lock();
	if (!l_ownerCache) { return; }

	std::shared_ptr<MRI::ComponentMode::InputComponentModeBase> l_inputComponentMode = nullptr;
	MRI::EditorUtility::FactoryRadioButtonSelector<MRI::SharedFactory::InputComponentMode>("InputComponentModeSelector" , l_inputComponentMode);

	// 既にリスト内で持っている"InputComponentMode"なら"return"
	if (!l_inputComponentMode)					 { return; }
	if (IsAlreadyContains(l_inputComponentMode)) { return; }

	// 初期化
	l_inputComponentMode->Init        ();
	l_inputComponentMode->PostLoadInit(l_ownerCache);

	m_inputComponentModeList.emplace_back(l_inputComponentMode);
}