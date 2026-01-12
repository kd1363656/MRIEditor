#include "MRIHermiteInterpolatorModifier.h"

#include "Application/main.h"

const MRI::TypeInfo& MRI::Modifier::HermiteInterpolatorModifier::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Modifier::HermiteInterpolatorModifier>();
}

void MRI::Modifier::HermiteInterpolatorModifier::Init()
{
	MRI::Modifier::InterpolatorModifierBase::Init();

	m_debugContextClickPos = {};

	m_debugDraggingSelectedIndex = k_debugInvalidIndex;
	m_debugDeleteSelectedIndex   = k_debugInvalidIndex;

	m_debugShowAddContext = false;
}

void MRI::Modifier::HermiteInterpolatorModifier::Update()
{
	const float l_deltaTime = Application::GetInstance().GetScaledDeltaTime();

	// 次のキーフレームがなければ実行しない(エラーになるから)
	if (m_keyFrameList.size() < k_minSizeToNextListAccess) { return; }

	const float l_elapsed = MRI::Modifier::InterpolatorModifierBase::GetElapsedTime();

	// 最後のキーを超えたら固定
	if (l_elapsed >= m_keyFrameList.back().time)
	{
		// 進捗率を"MAX(1.0f)"に固定
		MRI::Modifier::InterpolatorModifierBase::SetProgress    (MRI::CommonConstant::k_interpolatorProgressMax);
		MRI::Modifier::InterpolatorModifierBase::SetElapsedTime (m_keyFrameList.back().time);
		MRI::Modifier::InterpolatorModifierBase::SetCurrentValue(m_keyFrameList.back().value);
		return;
	}

	// 補完対象のキーフレームを探す
	// ("size() - 1ULL"で最大容量を超えないように調整)
	for (std::size_t l_i = 0ULL; l_i < m_keyFrameList.size() - 1ULL; ++l_i)
	{
		const auto& l_k0 = m_keyFrameList[l_i];
		const auto& l_k1 = m_keyFrameList[l_i + 1ULL];

		if (l_elapsed < l_k0.time || l_elapsed > l_k1.time) { continue; }

		// 気フレーム二点間と経過時間から進捗度を求める
		const float l_progressBetweenKeys = (l_elapsed - l_k0.time) / (l_k1.time - l_k0.time);

		// 進捗度を変数に格納
		MRI::Modifier::InterpolatorModifierBase::SetProgress(l_progressBetweenKeys);

		// エルミート補完
		const float l_currentValue = MRI::MathUtility::Hermite(l_progressBetweenKeys , 
															   l_k0.value			 , 
															   l_k1.value            , 
															   l_k0.tangent			 , 
															   l_k1.tangent);

		// エルミート補完で算出した補完結果値を格納
		MRI::Modifier::InterpolatorModifierBase::SetCurrentValue(l_currentValue);

		// 経過時間が現在の範囲を取るなら後の範囲を取ることは秒数が進まない限り不可能なので"break"
		break;
	}

	// 経過時間を加算したものを格納する
	MRI::Modifier::InterpolatorModifierBase::SetElapsedTime(l_elapsed + l_deltaTime);
}

void MRI::Modifier::HermiteInterpolatorModifier::EditPrefabInspector()
{
	MRI::Modifier::InterpolatorModifierBase::EditPrefabInspector();

	EditSortList ();
	EditDrawGraph();
}

void MRI::Modifier::HermiteInterpolatorModifier::DeserializePrefab(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::Modifier::InterpolatorModifierBase::DeserializePrefab(a_json);

	if (!MRI::JsonUtility::IsArray(a_json , "KeyFrameList")) { return; }
	
	for (const auto& l_jsonArray : a_json["KeyFrameList"])
	{
		MRI::Modifier::HermiteInterpolatorModifier::KeyFrame l_keyFrame = {};

		l_keyFrame.time    = l_jsonArray.value("Time"    , 0.0F);
		l_keyFrame.tangent = l_jsonArray.value("Tangent" , 0.0F);
		l_keyFrame.value   = l_jsonArray.value("Value"   , 0.0F);

		m_keyFrameList.emplace_back(l_keyFrame);
	}
}

nlohmann::json MRI::Modifier::HermiteInterpolatorModifier::SerializePrefab()
{
	auto l_rootJson  = nlohmann::json       ();
	auto l_jsonArray = nlohmann::json::array();

	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::Modifier::InterpolatorModifierBase::SerializePrefab());

	for (const auto& l_keyFrame : m_keyFrameList)
	{
		auto l_json = nlohmann::json();

		l_json["Time"]    = l_keyFrame.time;
		l_json["Tangent"] = l_keyFrame.tangent;
		l_json["Value"]   = l_keyFrame.value;

		l_jsonArray.emplace_back(l_json);
	}

	l_rootJson["KeyFrameList"] = l_jsonArray;

	return l_rootJson;
}

void MRI::Modifier::HermiteInterpolatorModifier::EditSortList()
{
	// 最低でもキーフレームが"2"点ないとソートをしない(エラーになるから)
	// 時間軸でキーフレームをソート
	if (m_keyFrameList.size() < k_minSizeToNextListAccess) { return; }

	std::sort(m_keyFrameList.begin() , m_keyFrameList.end() , [](const auto& a_compOne , const auto& a_compTwo) 
	{
		return a_compOne.time < a_compTwo.time;
	});

	for (auto& l_keyFrame : m_keyFrameList)
	{
		l_keyFrame.time  = std::min(l_keyFrame.time  , MRI::Modifier::InterpolatorModifierBase::GetMaxMeasurementTime());
		l_keyFrame.value = std::min(l_keyFrame.value , MRI::Modifier::InterpolatorModifierBase::GetMaxValue          ());
	}
}
void MRI::Modifier::HermiteInterpolatorModifier::EditDrawGraph()
{
	const ImVec2& l_canvasPos = ImGui::GetCursorScreenPos();	// 現在の"ImGui"ウィンドウの描画されている要素が終わった行の次の行の左上の座標を絶対座標で返す
	
	// "DrawList"を取得しグラフを描画
	ImDrawList* l_drawList = ImGui::GetWindowDrawList();
	if (!l_drawList) { return; }

	// "AddRect"や"AddLine"では左上の絶対座標、右下の絶対座標を指定するのが普通
	// 背景
	l_drawList->AddRect(l_canvasPos , l_canvasPos + m_graphSize , k_graphBorderColor);

	// 対角線ガイド
	l_drawList->AddLine(l_canvasPos, l_canvasPos + m_graphSize, k_graphGuidLineColor);

	EditDrawGridLine    (l_drawList , l_canvasPos);
	EditHermiteCurve    (l_drawList , l_canvasPos);
	EditDrawProgressLine(l_drawList , l_canvasPos);
	EditKeyFrame        (l_drawList , l_canvasPos);

	// サイズ変更
	EditGraphSize(l_drawList , l_canvasPos);
}
void MRI::Modifier::HermiteInterpolatorModifier::EditDrawProgressLine(ImDrawList* const a_drawList , const ImVec2& a_canvasPos) const
{
	if (!a_drawList) { return; }

	const float l_elapsedTime        = MRI::Modifier::InterpolatorModifierBase::GetElapsedTime       ();
	const float l_maxMeasurementTime = MRI::Modifier::InterpolatorModifierBase::GetMaxMeasurementTime();

	// 時間をグラフの上の"X"座標に変換
	const float l_x = a_canvasPos.x + (l_elapsedTime / l_maxMeasurementTime) * m_graphSize.x;

	// 線を上下に引く
	a_drawList->AddLine(ImVec2(l_x , a_canvasPos.y) , ImVec2(l_x , a_canvasPos.y + m_graphSize.y) , k_progressColor , k_lineThickness);
}
void MRI::Modifier::HermiteInterpolatorModifier::EditHermiteCurve(ImDrawList* const a_drawList , const ImVec2& a_canvasPos)
{
	// "DrawList"が"nullptr"か配列の要素が二つ以下なら実行しない(二点間の曲線を描画するものだから)
	if (!a_drawList || m_keyFrameList.size() < k_minSizeToNextListAccess) { return; }

	const float l_maxMeasurementTime = MRI::Modifier::InterpolatorModifierBase::GetMaxMeasurementTime();
	const float l_maxValue			 = MRI::Modifier::InterpolatorModifierBase::GetMaxValue			 ();

	for (std::size_t l_i = 0ULL; l_i < m_keyFrameList.size() - 1; ++l_i)
	{
		const auto& l_keyFrameFirst  = m_keyFrameList[l_i];
		const auto& l_keyFrameSecond = m_keyFrameList[l_i + 1ULL];

		// キーフレーム間をステップで補完し描画
		for (int l_step = 0; l_step < k_maxDebugStepNum; ++l_step)
		{
			const auto l_t0 = static_cast<float>(l_step)     / static_cast<float>(k_maxDebugStepNum);
			const auto l_t1 = static_cast<float>(l_step + 1) / static_cast<float>(k_maxDebugStepNum);

			const float l_time0 = l_keyFrameFirst.time + (l_keyFrameSecond.time - l_keyFrameFirst.time) * l_t0;
			const float l_time1 = l_keyFrameFirst.time + (l_keyFrameSecond.time - l_keyFrameFirst.time) * l_t1;

			const float l_val0 = MRI::MathUtility::Hermite(l_t0					   , 
													       l_keyFrameFirst.value   , 
														   l_keyFrameSecond.value  , 
														   l_keyFrameFirst.tangent , 
														   l_keyFrameSecond.tangent);

			const float l_val1 = MRI::MathUtility::Hermite(l_t1					   , 
													       l_keyFrameFirst.value   , 
														   l_keyFrameSecond.value  , 
														   l_keyFrameFirst.tangent , 
														   l_keyFrameSecond.tangent);

			const float l_x0 = a_canvasPos.x + (l_time0 / l_maxMeasurementTime) * m_graphSize.x;
			const float l_x1 = a_canvasPos.x + (l_time1 / l_maxMeasurementTime) * m_graphSize.x;

			const float l_y0 = a_canvasPos.y + ((l_maxValue - l_val0) / (2.0F * l_maxValue)) * m_graphSize.y;
			const float l_y1 = a_canvasPos.y + ((l_maxValue - l_val1) / (2.0F * l_maxValue)) * m_graphSize.y;

			a_drawList->AddLine(ImVec2(l_x0 , l_y0) , ImVec2(l_x1 , l_y1) , k_selectedKeyFrameColor , k_lineThickness);
		}
	}
}
void MRI::Modifier::HermiteInterpolatorModifier::EditKeyFrame(ImDrawList* const a_drawList , const ImVec2& a_canvasPos)
{
	if (!a_drawList) { return; }

	const ImVec2& l_mousePos = ImGui::GetIO().MousePos;

	const float l_maxMeasurementTime = MRI::Modifier::InterpolatorModifierBase::GetMaxMeasurementTime();
	const float l_maxValue           = MRI::Modifier::InterpolatorModifierBase::GetMaxValue          ();

	for (std::size_t l_i = 0ULL; l_i < m_keyFrameList.size(); ++l_i)
	{
		auto& l_key = m_keyFrameList[l_i];

		// キーフレームのスクリーン座標を求める
		ImVec2 l_keyPos = {};

		// 左上の座標("ImGui::Text"などの描画がない座標)から最大計測時間に占める時間倍率を算出し"canvasSize"と乗算することで正確な位置を算出
		l_keyPos.x = a_canvasPos.x + (l_key.time / l_maxMeasurementTime) * m_graphSize.x;
		l_keyPos.y = a_canvasPos.y + ((l_maxValue - l_key.value) / (2.0F * l_maxValue)) * m_graphSize.y;

		// もしマウス位置がキーフレームに近ければフラグは"true"
		bool l_isHovered = ImLengthSqr(l_mousePos - l_keyPos) <= ImLengthSqr(k_debugMouseHitRangeRadius);

		// キーフレームのかつ左クリックをして入ればドラッグしているキーフレームの"Index"を格納
		if (l_isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_debugDraggingSelectedIndex = static_cast<int>(l_i);
		}

		// キーフレームの上で右クリックされた場合、削除対象にする
		if (l_isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			m_debugDeleteSelectedIndex = static_cast<int>(l_i);
			ImGui::OpenPopup("DeleteKeyFrameContextMenu");
		}

		// ドラッグによる位置変更
		if (m_debugDraggingSelectedIndex == static_cast<int>(l_i) && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			const float l_newTime = (l_mousePos.x - a_canvasPos.x) / m_graphSize.x * l_maxMeasurementTime;
			const float l_newValue = (0.5F - ((l_mousePos.y - a_canvasPos.y) / m_graphSize.y)) * 2.0F * l_maxValue;

			l_key.time  = std::clamp(l_newTime  , 0.0F        , l_maxMeasurementTime);
			l_key.value = std::clamp(l_newValue , -l_maxValue , l_maxValue);

			// ツールチップで値を描画
			ImGui::BeginTooltip();
			ImGui::Text("Time    : %.2f" , l_key.time);
			ImGui::Text("Value   : %.2f" , l_key.value);
			ImGui::Text("Tangent : %.2f" , l_key.tangent);
			ImGui::EndTooltip();
		}

		// 色分け
		ImU32 l_color = (m_debugDraggingSelectedIndex == static_cast<int>(l_i)) ? k_selectedKeyFrameColor : k_defaultKeyFrameColor;

		a_drawList->AddCircleFilled(l_keyPos , k_debugCircleFiledRadius , l_color);
	}

	// マウスを話したらドラッグ解除
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		m_debugDraggingSelectedIndex = k_debugInvalidIndex;
	}

	// キーフレームの追加
	EditAddKeyFrame(l_mousePos , a_canvasPos);

	// キーフレームの削除
	EditDeleteKeyFrame();
}
void MRI::Modifier::HermiteInterpolatorModifier::EditAddKeyFrame(const ImVec2& a_mousePos , const ImVec2& a_canvasPos)
{
	// グラフ領域内で右クリックされたかつ削除予定の"Index"が無効な値じゃないかどうか
	if (const bool l_isInsideGraph = a_mousePos.x >= a_canvasPos.x && a_mousePos.x <= (a_canvasPos.x + m_graphSize.x) &&
									 a_mousePos.y >= a_canvasPos.y && a_mousePos.y <= (a_canvasPos.y + m_graphSize.y); 
		l_isInsideGraph && ImGui::IsMouseClicked(ImGuiMouseButton_Right) && m_debugDeleteSelectedIndex == k_debugInvalidIndex)
	{
		m_debugContextClickPos = a_mousePos;		// 追加位置を記録
		ImGui::OpenPopup("AddKeyFrameContextMenu"); 
	}

	if (ImGui::BeginPopup("AddKeyFrameContextMenu"))
	{
		if (ImGui::MenuItem(U8("追加")))
		{
			const float l_maxMeasurementTime = MRI::Modifier::InterpolatorModifierBase::GetMaxMeasurementTime();
			const float l_maxValue			 = MRI::Modifier::InterpolatorModifierBase::GetMaxValue			 ();

			// マウス位置をグラフ内座標に変換("0.0F ~ 1.0F"に正規化)
			MRI::Modifier::HermiteInterpolatorModifier::KeyFrame l_newKeyFrame = {};

			l_newKeyFrame.time    = (m_debugContextClickPos.x - a_canvasPos.x) / m_graphSize.x;
			l_newKeyFrame.value   = (0.5F - ((m_debugContextClickPos.y - a_canvasPos.y) / m_graphSize.y)) * 2.0F * l_maxValue;
			l_newKeyFrame.tangent = 0.0F;

			l_newKeyFrame.time  = std::clamp(l_newKeyFrame.time * l_maxMeasurementTime , 0.0F        , l_maxMeasurementTime);
			l_newKeyFrame.value = std::clamp(l_newKeyFrame.value					   , -l_maxValue , l_maxValue);

			m_keyFrameList.emplace_back(l_newKeyFrame);

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

}
void MRI::Modifier::HermiteInterpolatorModifier::EditDeleteKeyFrame()
{
	// もし無効な配列要素、容量超過、ポップアップのオープンがされていなければ"return"
	if (m_debugDeleteSelectedIndex == k_debugInvalidIndex)							   { return; }
	if (static_cast<std::size_t>(m_debugDeleteSelectedIndex) >= m_keyFrameList.size()) { return; }
	if (!ImGui::BeginPopup("DeleteKeyFrameContextMenu"))									   { return; }

	// 削除メニューの描画
	if (ImGui::MenuItem(U8("削除")))
	{
		m_keyFrameList.erase(m_keyFrameList.begin() + m_debugDeleteSelectedIndex);
		m_debugDeleteSelectedIndex = k_debugInvalidIndex;
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();
}
void MRI::Modifier::HermiteInterpolatorModifier::EditDrawGridLine(ImDrawList* const a_drawList , const ImVec2& a_canvasPos) const
{
	if (!a_drawList) { return; }

	const float l_maxMeasurementTime = MRI::Modifier::InterpolatorModifierBase::GetMaxMeasurementTime();

	for (int l_i = 1; l_i < k_maxDebugGridNum; ++l_i)
	{
		// "X"軸方向の縦線
		const float l_time = (l_maxMeasurementTime / k_maxDebugGridNum) * static_cast<float>(l_i);
		const float l_x    = a_canvasPos.x + ((l_time / l_maxMeasurementTime)  * m_graphSize.x);
		a_drawList->AddLine(ImVec2(l_x , a_canvasPos.y) , ImVec2(l_x , a_canvasPos.y + m_graphSize.y) , k_graphGuidLineColor);

		// "Y"軸方向の横線
		const float l_y = a_canvasPos.y + (m_graphSize.y / k_maxDebugGridNum) * static_cast<float>(l_i);
		a_drawList->AddLine(ImVec2(a_canvasPos.x , l_y) , ImVec2(a_canvasPos.x + m_graphSize.x , l_y) , k_graphGuidLineColor);
	}

	// "Y"軸"0"の位置を示す線
	const float l_yZero = a_canvasPos.y + m_graphSize.y * 0.5F;

	a_drawList->AddLine(ImVec2(a_canvasPos.x , l_yZero) , ImVec2(a_canvasPos.x + m_graphSize.x , l_yZero) , k_graphGuidYZeroColor);
}

void MRI::Modifier::HermiteInterpolatorModifier::EditGraphSize(ImDrawList* const a_drawList, const ImVec2& a_canvasPos)
{
	if (!a_drawList) { return; }

	// "InvisibleButton"のためにカーソル位置保存
	// "グラフの右下外にハンドルを表示"
	ImVec2 l_handlePos = a_canvasPos + m_graphSize + k_graphResizeHandleOffset;
	ImGui::SetCursorScreenPos(l_handlePos);

	// "InvisibleButton"でマウス操作を取得
	ImGui::InvisibleButton("##ResizeGraph" , k_resizeHandleSize , ImGuiButtonFlags_MouseButtonLeft);

	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		ImVec2 l_delta = ImGui::GetIO().MouseDelta;
		m_graphSize.x = std::max(m_graphSize.x + l_delta.x , k_graphSizeMin.x);
		m_graphSize.y = std::max(m_graphSize.y + l_delta.y , k_graphSizeMin.y);
	}

	// ハンドルのビジュアル
	a_drawList->AddRectFilled(l_handlePos , l_handlePos + k_resizeHandleSize , k_graphDragBoxColor);

	// "LayOut"上のスペース確保(他の"UI"と被ってしまうから)
	ImGui::SetCursorScreenPos(a_canvasPos);
	ImGui::Dummy             (m_graphSize);
}