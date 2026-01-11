#include "MRIRotationComponentByMouseModeBase.h"

#include "../../../../Application/main.h"

const MRI::TypeInfo& MRI::ComponentMode::RotationComponentByMouseModeBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentByMouseModeBase>();
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::Init()
{
	MRI::ComponentMode::RotationComponentModeBase::Init();

	m_targetRotation = Math::Vector3::Zero;

	m_rotationSpeed       =  k_defaultRotationSpeed;
	m_minRotatableDegreeX = -MRI::CommonConstant::k_quarterDegree;
	m_maxRotatableDegreeX =  MRI::CommonConstant::k_quarterDegree;
		
	m_disableMouseLock = false;
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::Update()
{
	// マウスの移動を中心に固定するかどうか
	MouseLock();

	if (m_disableMouseLock)      { return; }
	
	const auto& l_application = Application::GetInstance        ();
	const float l_deltaTime   = l_application.GetScaledDeltaTime();

	// ウィンドウの半分の大きさ
	const MRI::CommonStruct::Dimension2D& l_windowHalfSize = { l_application.GetWindowSize().width  / 2 ,
															   l_application.GetWindowSize().height / 2 };

	// 今のマウス座標をスクリーン座標で取得(デスクトップの左上を基準)
	POINT l_nowMousePos = {};
	GetCursorPos(&l_nowMousePos);

	// ウィンドウハンドル(現在のウィンドウのハンドルを取得)
	const auto& l_hWND = l_application.GetHWND();

	// スクリーン座標からクライアント座標に変換(タイトルバーを含まない現在のウィンドウの左上を基準とした座標を取得)
	ScreenToClient(l_hWND , &l_nowMousePos);

	// マウスの移動量(現在のマウス位置から画面の中央座標を引いた差分)
	Math::Vector2 l_movement = { static_cast<float>(l_nowMousePos.x - l_windowHalfSize.width) ,
								 static_cast<float>(l_nowMousePos.y - l_windowHalfSize.height) };

	// クライアント座標の中心をスクリーン座標に変換
	POINT l_centerScreenPos = { l_windowHalfSize.width , l_windowHalfSize.height };

	// クライアント座標の中心をスクリーン座標に変換
	ClientToScreen(l_hWND , &l_centerScreenPos);

	// マウスをクライアント画面中心に戻す
	SetCursorPos(l_centerScreenPos.x , l_centerScreenPos.y);

	// マウスの移動量がほとんどないなら補完の進捗をリセットして"return"
	if (l_movement.LengthSquared() < CommonConstant::k_epsilon) { return; }
	
	const float l_rotationSpeed = m_rotationSpeed * l_deltaTime;

	// ターゲット回転からオイラー角を取得
	Math::Vector3 l_rotation = m_targetRotation;

	// マウス移動をオイラー角に加算("X = 上下" , "Y = "左右"")
	l_rotation.x += l_movement.y * l_rotationSpeed;
	l_rotation.y += l_movement.x * l_rotationSpeed;

	// "X"軸の回転にだけ制限を掛ける
	l_rotation.x = std::clamp(l_rotation.x , m_minRotatableDegreeX , m_maxRotatableDegreeX);
	
	// 向くべき方向を格納
	m_targetRotation = l_rotation;	
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::EditPrefabInspector()
{
	MRI::ComponentMode::RotationComponentModeBase::EditPrefabInspector();

	ImGui::DragFloat("RotationSpeed" , &m_rotationSpeed , MRI::EditorCommonConstant::k_defaultDragValue);
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::ComponentMode::RotationComponentModeBase::DeserializePrefab(a_json);

	m_rotationSpeed       = a_json.value("RotationSpeed"       ,  k_defaultRotationSpeed);
	m_minRotatableDegreeX = a_json.value("MinRotatableDegreeX" , -MRI::CommonConstant::k_quarterDegree);
	m_maxRotatableDegreeX = a_json.value("MaxRotatableDegreeX" ,  MRI::CommonConstant::k_quarterDegree);
}

nlohmann::json MRI::ComponentMode::RotationComponentByMouseModeBase::SerializePrefab()
{
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::ComponentMode::RotationComponentModeBase::SerializePrefab());

	l_rootJson["RotationSpeed"]       = m_rotationSpeed;
	l_rootJson["MinRotatableDegreeX"] = m_minRotatableDegreeX;
	l_rootJson["MaxRotatableDegreeX"] = m_maxRotatableDegreeX;
	
	return l_rootJson;
}

void MRI::ComponentMode::RotationComponentByMouseModeBase::MouseLock()
{
	// 一回だけキー入力に反応してトグル操作を行う
	if (const auto& l_input = MRI::InputManager::GetInstance();
		l_input.IsInputOnce(VK_TAB))
	{
		m_disableMouseLock = m_disableMouseLock ? false : true;
	}
}