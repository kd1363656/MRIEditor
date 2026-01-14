#include "MRITransformComponent.h"

MRI::Component::TransformComponent::TransformComponent () = default;
MRI::Component::TransformComponent::~TransformComponent() = default;

const MRI::TypeInfo& MRI::Component::TransformComponent::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::TransformComponent>();
}

void MRI::Component::TransformComponent::Init()
{
	m_transform = {};
}

void MRI::Component::TransformComponent::PostLoadInit()
{
	auto l_ownerCache = MRI::Component::ComponentBase::GetWorkOwnerCache().lock();
	if (!l_ownerCache) { return; }

	// 親がいたら親の"TransformComponent"をキャッシュする
	if (auto l_parentCache = l_ownerCache->GetParentCache().lock())
	{
		m_parentTransformComponentCache = l_parentCache->GetSelfTransformComponentCache().lock();
	}

	// 行列の更新
	FixMatrixStrategy();
}

void MRI::Component::TransformComponent::EditSpawnInspector()
{
	bool l_hasParamChanged = EditCommonInspector();

	if (ImGui::DragFloat3(U8("座標") , &m_transform.pos.x , MRI::EditorCommonConstant::k_defaultDragValue))
	{
		l_hasParamChanged = true;
	}

	// もし"Transform"が編集されたらその変更を視覚的に反映するために行列を計算
	if (l_hasParamChanged)
	{
		FixMatrixStrategy();
	}
}
void MRI::Component::TransformComponent::EditPrefabInspector()
{
	MRI::EditorUtility::FactoryRadioButtonSelector<MRI::UniqueFactory::Strategy<MRI::Strategy::MatrixStrategyBase>>("MatrixStrategy" , m_fixMatrixStrategy);

	const bool l_hasParamChanged = EditCommonInspector();

	// もし"Transform"が編集されたらその変更を視覚的に反映するために行列を計算
	if (l_hasParamChanged)
	{
		FixMatrixStrategy();
	}
}

void MRI::Component::TransformComponent::DeserializeSpawn(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }	
	DeserializeCommon(a_json);
}
void MRI::Component::TransformComponent::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }
	DeserializeCommon(a_json);

	const auto& l_matrixStrategyFactory = MRI::UniqueFactory::Strategy<MRI::Strategy::MatrixStrategyBase>::GetInstance();

	// "json"からストラテジーの名前を取得してファクトリーを通してインスタンスを生成
	const std::string l_matrixStrategyName = a_json.value				   ("MatrixStrategyName" , std::string());
	m_fixMatrixStrategy					   = l_matrixStrategyFactory.Create(l_matrixStrategyName);
}

nlohmann::json MRI::Component::TransformComponent::SerializeSpawn()
{
	auto l_rootJson = nlohmann::json();
	MRI::JsonUtility::UpdateJson    (l_rootJson , SerializeCommon());

	return l_rootJson;
}
nlohmann::json MRI::Component::TransformComponent::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();
	MRI::JsonUtility::UpdateJson    (l_rootJson , SerializeCommon());

	// ストラテジーの名前を保存
	if (m_fixMatrixStrategy)
	{
		const std::string l_matrixStrategyName = m_fixMatrixStrategy->GetTypeInfo().k_name.data();
		l_rootJson["MatrixStrategyName"]       = l_matrixStrategyName;
	}

	return l_rootJson;
}

void MRI::Component::TransformComponent::FixMatrix()
{
	FixMatrixStrategy();
}

bool MRI::Component::TransformComponent::EditCommonInspector()
{
	bool l_hasParamChanged = false;

	Math::Vector3 l_euler = MRI::MathUtility::QuaternionToEuler(m_transform.rotation);

	// 拡大率を操作
	if (ImGui::DragFloat3(U8("拡大") , &m_transform.scale.x , MRI::EditorCommonConstant::k_defaultDragValue))
	{
		l_hasParamChanged = true;
	}

	// 回転率を操作
	if (ImGui::DragFloat3(U8("回転") , &l_euler.x , MRI::EditorCommonConstant::k_defaultDragValue))
	{
		// オイラー角に変換していたクオータニオンをオイラー角に戻して格納
		Math::Quaternion l_dragResult = MRI::MathUtility::EulerToQuaternion(l_euler);
		m_transform.rotation          = l_dragResult;

		l_hasParamChanged = true;
	}

	return l_hasParamChanged;
}

void MRI::Component::TransformComponent::DeserializeCommon(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	m_transform.scale    = MRI::JsonUtility::DeserializeVec3      (a_json , "Scale");
	m_transform.rotation = MRI::JsonUtility::DeserializeQuaternion(a_json , "Rotation");
	m_transform.pos      = MRI::JsonUtility::DeserializeVec3      (a_json , "Pos");
}

nlohmann::json MRI::Component::TransformComponent::SerializeCommon() const
{
	auto l_rootJson = nlohmann::json();

	auto l_scaleJson    = MRI::JsonUtility::SerializeVec3      (m_transform.scale   , "Scale");
	auto l_rotationJson = MRI::JsonUtility::SerializeQuaternion(m_transform.rotation, "Rotation");
	auto l_posJson      = MRI::JsonUtility::SerializeVec3      (m_transform.pos     , "Pos");

	MRI::JsonUtility::UpdateJson(l_rootJson , l_scaleJson);
	MRI::JsonUtility::UpdateJson(l_rootJson , l_rotationJson);
	MRI::JsonUtility::UpdateJson(l_rootJson , l_posJson);
	
	return l_rootJson;
}

void MRI::Component::TransformComponent::FixMatrixStrategy()
{
	if (!m_fixMatrixStrategy) { return; }

	auto l_self = weak_from_this();
	m_fixMatrixStrategy->Execute(l_self);
}