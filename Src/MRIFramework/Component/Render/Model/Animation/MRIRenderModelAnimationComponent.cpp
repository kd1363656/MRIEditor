#include "MRIRenderModelAnimationComponent.h"

const MRI::TypeInfo& MRI::Component::RenderModelAnimationComponent::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::RenderModelAnimationComponent>();
}

void MRI::Component::RenderModelAnimationComponent::Init()
{
	MRI::Component::RenderModelComponentBase::Init();

	if (!m_modelWork)
	{
		m_modelWork = std::make_shared<KdModelWork>();
	}
}

void MRI::Component::RenderModelAnimationComponent::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }
	if (!m_modelWork)	  { return; }

	MRI::Component::RenderModelComponentBase::DeserializePrefab(a_json);

	auto l_assetFilePathHelperCache = MRI::Component::RenderModelComponentBase::GetAssetFilePathHelperCache().lock();
	if (!l_assetFilePathHelperCache) { return; }

	// 空のファイルパス出なければ実行
	if (const std::string& l_filePath = l_assetFilePathHelperCache->GetFilePath();
		!l_filePath.empty()) 
	{
		m_modelWork->SetModelData(l_assetFilePathHelperCache->GetFilePath());
	}
}

void MRI::Component::RenderModelAnimationComponent::EditPrefabInspector()
{
	if (!m_modelWork) { return; }

	MRI::Component::RenderModelComponentBase::EditPrefabInspector();

	auto l_assetFilePathHelperCache = MRI::Component::RenderModelComponentBase::GetAssetFilePathHelperCache().lock();
	if (!l_assetFilePathHelperCache) { return; }

	// アセットのファイルパスが変更されたらモデルを再度読み込む
	if (l_assetFilePathHelperCache->GetIsChanged())
	{
		m_modelWork->SetModelData(l_assetFilePathHelperCache->GetFilePath());
	}
}

void MRI::Component::RenderModelAnimationComponent::Draw(const std::uint32_t a_shaderTag)
{
	// もし使用可能なシェーダーであるなら処理を実行
	if (!MRI::Component::RenderModelComponentBase::CanUseShader(a_shaderTag)) { return; }

	auto l_selfTransformComponentCache = MRI::Component::RenderModelComponentBase::GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	if (!m_modelWork) { return; }
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_modelWork , l_selfTransformComponentCache->GetMatrix() , MRI::Component::RenderModelComponentBase::GetColor());
}