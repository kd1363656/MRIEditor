#include "MRIRotationComponentModeBase.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentModeBase>();
}

void MRI::ComponentMode::RotationComponentModeBase::Init()
{
	m_adaptRotationDirectionTagList.clear();

	m_selfTransformComponentCache.reset();

	m_rotationDirection = Math::Vector3::Zero;

	m_debugAddRotationDirectionAdaptTag = MRI::CommonConstant::k_invalidStaticID;
}
void MRI::ComponentMode::RotationComponentModeBase::PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner)
{
	if (!a_owner) { return; }

	m_selfTransformComponentCache = a_owner->GetSelfTransformComponentCache();
}

void MRI::ComponentMode::RotationComponentModeBase::PreUpdate()
{
	// 外部から受け取った回転方向をクリア
	m_rotationDirection = Math::Vector3::Zero;
}

void MRI::ComponentMode::RotationComponentModeBase::EditPrefabInspector()
{
	auto l_itr = m_adaptRotationDirectionTagList.begin();
	if (ImGui::CollapsingHeader("RotationDirectionAdaptList"))
	{
		EditAddAdaptRotationDirectionTag();

		while (l_itr != m_adaptRotationDirectionTagList.end())
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
				ImGui::PopID								 ();
				l_itr = m_adaptRotationDirectionTagList.erase(l_itr);
				continue;
			}

			l_itr++;
			ImGui::PopID();
		}
	}
}

void MRI::ComponentMode::RotationComponentModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null())													 { return; }
	if (!MRI::JsonUtility::IsArray(a_json, "AdaptRotationDirectionTagList")) { return; }

	for (const auto& l_json : a_json["AdaptRotationDirectionTagList"])
	{
		std::uint32_t l_tag = MRI::JsonUtility::DeserializeTag(l_json);
		if (l_tag == MRI::CommonConstant::k_invalidStaticID) { continue; }

		RegisterAdaptRotationDirectionTag(l_tag);
	}
}

nlohmann::json MRI::ComponentMode::RotationComponentModeBase::SerializePrefab()
{
	auto l_rootJson  = nlohmann::json       ();
	auto l_jsonArray = nlohmann::json::array();

	for (const auto& l_tag : m_adaptRotationDirectionTagList)
	{
		const auto& l_json = MRI::JsonUtility::SerializeTag(l_tag);
		if (l_json.is_null()) { continue; }

		l_jsonArray.emplace_back(l_json);
	}

	l_rootJson["AdaptRotationDirectionTagList"] = l_jsonArray;

	return l_rootJson;
}

void MRI::ComponentMode::RotationComponentModeBase::RegisterAdaptRotationDirectionTag(const std::uint32_t a_addRotationDirectionAdaptTag)
{
	if (a_addRotationDirectionAdaptTag != MRI::CommonConstant::k_invalidStaticID) { return; }
	m_adaptRotationDirectionTagList.emplace(a_addRotationDirectionAdaptTag);
}

void MRI::ComponentMode::RotationComponentModeBase::EditAddAdaptRotationDirectionTag()
{
	// ラジオボタンと追加のボタンの描画
	MRI::EditorUtility::TagRadioButtonSelector<MRI::Tag::AxisTagBase>("AdaptRotationDirectionTagSelector", m_debugAddRotationDirectionAdaptTag);

	if (m_debugAddRotationDirectionAdaptTag != MRI::CommonConstant::k_invalidStaticID) { return; }

	if (!MRI::EditorUtility::SameLineButton("Add AdaptRotationDirectionTag")) { return; }
	RegisterAdaptRotationDirectionTag(m_debugAddRotationDirectionAdaptTag);
}

bool MRI::ComponentMode::RotationComponentModeBase::CanAdaptRotationDirection(const std::uint32_t a_tag) const
{
	return m_adaptRotationDirectionTagList.contains(a_tag);
}