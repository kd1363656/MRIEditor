#include "MRICameraComponent.h"

const MRI::TypeInfo& MRI::Component::CameraComponent::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::CameraComponent>();
}

void MRI::Component::CameraComponent::Init()
{
	if (!m_camera) 
	{
		m_camera = std::make_shared<KdCamera>();
	}

	m_fieldOfView = k_defaultFieldOfView;

	m_cameraTag = MRI::CommonConstant::k_invalidStaticID;
}
void MRI::Component::CameraComponent::PostLoadInit()
{
	auto l_ownerCache = MRI::Component::ComponentBase::GetWorkOwnerCache().lock();

	if (!l_ownerCache) { return; }
	if (!m_camera)     { return; }

	// "json"から視野角を読み取ってから実際に視野角を設定
	m_camera->SetProjectionMatrix(m_fieldOfView);

	m_selfTransformComponentCache = l_ownerCache->GetSelfTransformComponentCache();
}

void MRI::Component::CameraComponent::EditSpawnInspector()
{
	ImGui::DragFloat("FiledOfView"                                 , 
					 &m_fieldOfView                                , 
					 MRI::EditorCommonConstant::k_defaultDragValue , 
					 k_minFieldOfView							   , 
					 k_maxFieldOfView);
}
void MRI::Component::CameraComponent::EditPrefabInspector()
{
	MRI::EditorUtility::TagRadioButtonSelector<MRI::Tag::CameraTagBase>("SelectCameraTag" , m_cameraTag);
}

void MRI::Component::CameraComponent::DeserializeSpawn(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }
	m_fieldOfView = a_json.value("FieldOfView" , k_defaultFieldOfView);
}
void MRI::Component::CameraComponent::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }
	m_cameraTag = MRI::JsonUtility::DeserializeTag(a_json);
}

nlohmann::json MRI::Component::CameraComponent::SerializeSpawn()
{
	auto l_rootJson = nlohmann::json();

	l_rootJson["FieldOfView"] = m_fieldOfView;

	return l_rootJson;
}
nlohmann::json MRI::Component::CameraComponent::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();
	
	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::JsonUtility::SerializeTag(m_cameraTag));

	return l_rootJson;
}

void MRI::Component::CameraComponent::PreDraw()
{
	auto l_selfTransformComponentCache = m_selfTransformComponentCache.lock();
	if (!l_selfTransformComponentCache) { return; }
	if (!m_camera)				   { return; }

	m_camera->SetCameraMatrix(l_selfTransformComponentCache->GetMatrix());
	m_camera->SetToShader    ();

	// 視錐台のキャッシュ作成、描画された後にすぐに行わないと
	// 後の描画のカリングに使えず意味がないからここで実行
	CreateFrustum();
}

void MRI::Component::CameraComponent::CreateFrustum()
{
	if (!m_camera) { return; }

	auto l_ownerCache = MRI::Component::ComponentBase::GetWorkOwnerCache().lock();
	if (!l_ownerCache) { return; }

	auto l_selfTransformComponentCache = l_ownerCache->GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	// 投射行列から視錐台を初期化
	DirectX::BoundingFrustum::CreateFromMatrix(m_boundingFrustum , m_camera->GetProjMatrix());

	// カメラの行列から位置・回転・スケールを一括適用
	DirectX::BoundingFrustum::CreateFromMatrix(m_boundingFrustum , l_selfTransformComponentCache->GetMatrix());
}