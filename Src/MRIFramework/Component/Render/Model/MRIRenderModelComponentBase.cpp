#include "MRIRenderModelComponentBase.h"

const MRI::TypeInfo& MRI::Component::RenderModelComponentBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::RenderModelComponentBase>();
}

void MRI::Component::RenderModelComponentBase::Init()
{
	MRI::RenderManager::GetInstance().AddRenderModelComponentBase(weak_from_this());

	// フィルター情報は各派生クラスで使う("gltf"や"png"どちらを選ぶか現状分からないから)
	if (!m_assetFilePathHelper)
	{
		m_assetFilePathHelper = std::make_shared<MRI::Helper::AssetFilePathHelper>();
	}

	m_assetFilePathHelper->Init();

	m_shaderTagSet.clear();

	m_debugAddSelectedTag = MRI::CommonConstant::k_invalidStaticID;

	// アセットファイルパスのフィルター情報をセット
	m_assetFilePathHelper->SetFilters(MRI::EditorCommonConstant::k_gltfFilePathFilter);
}

void MRI::Component::RenderModelComponentBase::PostLoadInit() 
{
	auto l_ownerCache = MRI::Component::ComponentBase::GetWorkOwnerCache().lock();
	if (!l_ownerCache) { return; }

	m_selfTransformComponentCache = l_ownerCache->GetSelfTransformComponentCache();
}

void MRI::Component::RenderModelComponentBase::EditPrefabInspector()
{
	if (!m_assetFilePathHelper) { return; }

	MRI::EditorUtility::TagRadioButtonSelector<MRI::Tag::ShaderTagBase>("SelectShaderTypeTag" , m_debugAddSelectedTag);

	// ボタンが押されたら指定された"Shader"の"Tag"を追加
	if (MRI::EditorUtility::SameLineButton("Add UseShaderTag"))
	{
		RegisterShaderTag(m_debugAddSelectedTag);
	}

	ImGui::Separator();

	// カラー編集
	ImGui::ColorEdit4("Color", &m_color.x);

	// 使用しているシェーダーの描画
	if (ImGui::CollapsingHeader("UseShader"))
	{
		auto  l_itr = m_shaderTagSet.begin();

		while (l_itr != m_shaderTagSet.end())
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
				ImGui::PopID();
				l_itr = m_shaderTagSet.erase(l_itr);
				continue;
			}

			ImGui::PopID();
			l_itr++;
		}
	}

	m_assetFilePathHelper->EditPrefabInspector();
}

void MRI::Component::RenderModelComponentBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null())									 { return; }
	if (!MRI::JsonUtility::IsArray(a_json, "ShaderTagList")) { return; }
	if (!m_assetFilePathHelper)								 { return; }

	if (m_assetFilePathHelper)
	{
		m_assetFilePathHelper->DeserializePrefab(a_json);
	}

	m_color = MRI::JsonUtility::DeserializeColor(a_json , "Color");

	for (const auto& l_json : a_json["ShaderTagList"])
	{
		if (l_json.is_null()) { continue; }

		uint32_t l_shaderTag = MRI::JsonUtility::DeserializeTag(l_json);

		// もし無効な"ID"なら登録しない
		if (l_shaderTag == MRI::CommonConstant::k_invalidStaticID) { continue; }

		m_shaderTagSet.emplace(l_shaderTag);
	}

	m_assetFilePathHelper->DeserializePrefab(a_json);
}
nlohmann::json MRI::Component::RenderModelComponentBase::SerializePrefab()
{
	if (!m_assetFilePathHelper)
	{
		return nlohmann::json(); 
	}

	auto l_rootJson  = nlohmann::json		();
	auto l_jsonArray = nlohmann::json::array();

	if (m_assetFilePathHelper)
	{
		MRI::JsonUtility::UpdateJson(l_rootJson , m_assetFilePathHelper->SerializePrefab());
	}

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::JsonUtility::SerializeColor(m_color , "Color"));

	// シェーダータグリストのシリアライズ
	for (const auto& l_shaderTag : m_shaderTagSet)
	{
		if (l_shaderTag == MRI::CommonConstant::k_invalidStaticID) { continue; }

		auto l_json = nlohmann::json();

		MRI::JsonUtility::UpdateJson(l_json , MRI::JsonUtility::SerializeTag(l_shaderTag));

		if (l_json.is_null()) { continue; }
		l_jsonArray.emplace_back(l_json);
	}
	
	l_rootJson["ShaderTagList"] = l_jsonArray;

	MRI::JsonUtility::UpdateJson(l_rootJson , m_assetFilePathHelper->SerializePrefab());

	return l_rootJson;
}

void MRI::Component::RenderModelComponentBase::RegisterShaderTag(const std::uint32_t a_shaderTag)
{
	if (a_shaderTag == MRI::CommonConstant::k_invalidStaticID) { return; }
	m_shaderTagSet.emplace(a_shaderTag);
}

bool MRI::Component::RenderModelComponentBase::CanUseShader(const std::uint32_t a_shaderTag) const
{
	if (a_shaderTag == MRI::CommonConstant::k_invalidStaticID) { return false; }
	return m_shaderTagSet.contains(a_shaderTag);
}