#include "MRIMoveComponentModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::MoveComponentModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentModeBase>();
}

void MRI::ComponentMode::MoveComponentModeBase::Init()
{
	m_adaptMoveAxisTagSet.clear();

	m_selfTransformComponentCache.reset();

	m_moveDirection = Math::Vector3::Zero;

	m_debugAddMoveAxisAdaptTag = MRI::CommonConstant::k_invalidStaticID;
}
void MRI::ComponentMode::MoveComponentModeBase::PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner)
{
	if (!a_owner) { return; }

	m_selfTransformComponentCache = a_owner->GetSelfTransformComponentCache();
}

void MRI::ComponentMode::MoveComponentModeBase::EditPrefabInspector()
{
	if (ImGui::CollapsingHeader("MoveAxisAdaptSet"))
	{
		EditAddAdaptMoveDirectionTag();

		auto l_itr = m_adaptMoveAxisTagSet.begin();
		while (l_itr != m_adaptMoveAxisTagSet.end())
		{
			// タグ"ID"から型情報を取得
			std::uint32_t l_tagID = *l_itr;
			if (l_tagID == MRI::CommonConstant::k_invalidStaticID) 
			{
				l_itr++;
				continue; 
			}

			const auto* l_typeInfo = MRI::TypeInfoRegistry::GetInstance().FetchTypeInfoByID(l_tagID);
			if (!l_typeInfo) 
			{
				l_itr++;
				continue; 
			}

			ImGui::PushID(std::to_address(l_itr));

			ImGui::Text(l_typeInfo->k_name.data());

			if (MRI::EditorUtility::SameLineSmallButton("Delete"))
			{
				ImGui::PopID					        ();
				l_itr = m_adaptMoveAxisTagSet.erase(l_itr);
				continue;
			}

			l_itr++;
			ImGui::PopID();
		}
	}
}

void MRI::ComponentMode::MoveComponentModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null())										    { return; }
	if (!MRI::JsonUtility::IsArray(a_json , "AdaptMoveAxisTagSet")) { return; }

	for (const auto& l_json : a_json["AdaptMoveAxisTagSet"])
	{
		std::uint32_t l_tag = MRI::JsonUtility::DeserializeTag(l_json);
		if (l_tag == MRI::CommonConstant::k_invalidStaticID) { continue; }

		RegisterAdaptMoveAxisTag(l_tag);
	}
}

nlohmann::json MRI::ComponentMode::MoveComponentModeBase::SerializePrefab()
{
	auto l_rootJson  = nlohmann::json       ();
	auto l_jsonArray = nlohmann::json::array();

	for (const auto& l_tag : m_adaptMoveAxisTagSet)
	{
		const auto& l_json = MRI::JsonUtility::SerializeTag(l_tag);
		if (l_json.is_null()) { continue; }

		l_jsonArray.emplace_back(l_json);
	}

	l_rootJson["AdaptMoveAxisTagSet"] = l_jsonArray;

	return l_rootJson;
}

void MRI::ComponentMode::MoveComponentModeBase::ResetMoveDirection()
{
	m_moveDirection = Math::Vector3::Zero;
}

void MRI::ComponentMode::MoveComponentModeBase::RegisterAdaptMoveAxisTag(const std::uint32_t a_addMoveAxisAdaptTag)
{
	if (a_addMoveAxisAdaptTag == MRI::CommonConstant::k_invalidStaticID) { return; }
	m_adaptMoveAxisTagSet.emplace(a_addMoveAxisAdaptTag);
}

void MRI::ComponentMode::MoveComponentModeBase::EditAddAdaptMoveDirectionTag()
{
	// ラジオボタンと追加のボタンの描画
	MRI::EditorUtility::TagRadioButtonSelector<MRI::Tag::AxisTagBase>("AdaptMoveAxisTagSelector" , m_debugAddMoveAxisAdaptTag);

	if (!MRI::EditorUtility::SameLineButton("Add AdaptMoveAxisTag")) { return; }
	RegisterAdaptMoveAxisTag(m_debugAddMoveAxisAdaptTag);
}

Math::Vector3 MRI::ComponentMode::MoveComponentModeBase::CalcMoveDirection()
{
	auto l_selfTransformComponentCache = MRI::ComponentMode::MoveComponentModeBase::GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) 
	{
		return Math::Vector3::Zero; 
	}

	auto l_moveDirection = MRI::ComponentMode::MoveComponentModeBase::GetMoveDirection();

	// 移動方向が格納されていないか値が小さければ"return"
	if (l_moveDirection.LengthSquared() <= MRI::CommonConstant::k_epsilon) 
	{
		return Math::Vector3::Zero; 
	}

	// 移動方向を正規化
	l_moveDirection.Normalize();

	// このゲームオブジェクトの正面方向から見て向くべき方向を計算
	Math::Matrix  l_rotationMatrix      = l_selfTransformComponentCache->CalcRotationMatrix();
	Math::Vector3 l_resultMoveDirection = Math::Vector3::TransformNormal                   (l_moveDirection , l_rotationMatrix);

	// 使用しない軸の値を"0.0F"にして使用しないようにする
	MRI::AxisUtility::ResetUnusedAxis(m_adaptMoveAxisTagSet , l_resultMoveDirection);
	return l_resultMoveDirection;
}