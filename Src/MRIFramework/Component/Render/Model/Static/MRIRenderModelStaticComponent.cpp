#include "MRIRenderModelStaticComponent.h"

const MRI::TypeInfo& MRI::Component::RenderModelStaticComponent::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::RenderModelStaticComponent>();
}

void MRI::Component::RenderModelStaticComponent::Init()
{
	MRI::Component::RenderModelComponentBase::Init();

	if (!m_modelData)
	{
		m_modelData = std::make_shared<KdModelData>();
	}
}

void MRI::Component::RenderModelStaticComponent::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::Component::RenderModelComponentBase::DeserializePrefab(a_json);

	auto l_assetFilePathHelperCache = MRI::Component::RenderModelStaticComponent::GetAssetFilePathHelperCache().lock();
	if (!l_assetFilePathHelperCache) { return; }

	// 空のファイルパス出なければ実行
	if (const std::string& l_filePath = l_assetFilePathHelperCache->GetFilePath();
		!l_filePath.empty())
	{
		m_modelData->Load(l_assetFilePathHelperCache->GetFilePath());
	}
}

void MRI::Component::RenderModelStaticComponent::Draw(const std::uint32_t a_shaderTag)
{
	// もし使用可能なシェーダーであるなら処理を実行
	if (!MRI::Component::RenderModelStaticComponent::CanUseShader(a_shaderTag)) { return; }

	auto l_selfTransformComponentCache = MRI::Component::RenderModelComponentBase::GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	if (!m_modelData) { return; }

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_modelData, l_selfTransformComponentCache->GetMatrix(), MRI::Component::RenderModelStaticComponent::GetColor());
}