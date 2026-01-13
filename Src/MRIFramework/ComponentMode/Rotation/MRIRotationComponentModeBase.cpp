#include "MRIRotationComponentModeBase.h"

#include "../../../Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentModeBase>();
}

void MRI::ComponentMode::RotationComponentModeBase::Init()
{
	const auto& l_application = Application::GetInstance();

	// マウスカーソルをクライアント座標の中心にセット
	MRI::CommonStruct::Dimension2D l_halfWindowSize = { l_application.GetWindowSize().width / 2, l_application.GetWindowSize().height / 2 };

	// クライアント座標の中心をスクリーン座標に変換
	POINT l_centerScreenPos = { l_halfWindowSize.width , l_halfWindowSize.height };

	// クライアント座標の中心をスクリーン座標に変換
	ClientToScreen(l_application.GetHWND() , &l_centerScreenPos);

	// マウスをクライアント画面中心に戻す
	SetCursorPos(l_centerScreenPos.x , l_centerScreenPos.y);

	m_adaptRotationAxisTagSet.clear();

	m_selfTransformComponentCache.reset();

	m_rotationDirection = Math::Vector3::Zero;

	m_debugAddRotationAxisAdaptTag = MRI::CommonConstant::k_invalidStaticID;
}
void MRI::ComponentMode::RotationComponentModeBase::PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner)
{
	if (!a_owner) { return; }

	m_selfTransformComponentCache = a_owner->GetSelfTransformComponentCache();
}

void MRI::ComponentMode::RotationComponentModeBase::EditPrefabInspector()
{
	if (ImGui::CollapsingHeader("RotationAxisAdaptSet"))
	{
		EditAddAdaptRotationAxisTag();

		auto l_itr = m_adaptRotationAxisTagSet.begin();
		while (l_itr != m_adaptRotationAxisTagSet.end())
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
				l_itr = m_adaptRotationAxisTagSet.erase(l_itr);
				continue;
			}

			l_itr++;
			ImGui::PopID();
		}
	}
}

void MRI::ComponentMode::RotationComponentModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null())										       { return; }
	if (!MRI::JsonUtility::IsArray(a_json, "AdaptRotationAxisTagSet")) { return; }

	for (const auto& l_json : a_json["AdaptRotationAxisTagSet"])
	{
		std::uint32_t l_tag = MRI::JsonUtility::DeserializeTag(l_json);
		if (l_tag == MRI::CommonConstant::k_invalidStaticID) { continue; }

		RegisterAdaptRotationAxisTag(l_tag);
	}
}

nlohmann::json MRI::ComponentMode::RotationComponentModeBase::SerializePrefab()
{
	auto l_rootJson  = nlohmann::json       ();
	auto l_jsonArray = nlohmann::json::array();

	for (const auto& l_tag : m_adaptRotationAxisTagSet)
	{
		const auto& l_json = MRI::JsonUtility::SerializeTag(l_tag);
		if (l_json.is_null()) { continue; }

		l_jsonArray.emplace_back(l_json);
	}

	l_rootJson["AdaptRotationAxisTagSet"] = l_jsonArray;

	return l_rootJson;
}

void MRI::ComponentMode::RotationComponentModeBase::ResetRotationDirection()
{
	m_rotationDirection = Math::Vector3::Zero;
}

void MRI::ComponentMode::RotationComponentModeBase::RegisterAdaptRotationAxisTag(const std::uint32_t a_addRotationAxisAdaptTag)
{
	if (a_addRotationAxisAdaptTag == MRI::CommonConstant::k_invalidStaticID) { return; }
	m_adaptRotationAxisTagSet.emplace(a_addRotationAxisAdaptTag);
}

void MRI::ComponentMode::RotationComponentModeBase::EditAddAdaptRotationAxisTag()
{
	// ラジオボタンと追加のボタンの描画
	MRI::EditorUtility::TagRadioButtonSelector<MRI::Tag::AxisTagBase>("AdaptRotationAxisTagSelector", m_debugAddRotationAxisAdaptTag);

	if (!MRI::EditorUtility::SameLineButton("Add AdaptRotationAxisTag")) { return; }
	RegisterAdaptRotationAxisTag(m_debugAddRotationAxisAdaptTag);
}