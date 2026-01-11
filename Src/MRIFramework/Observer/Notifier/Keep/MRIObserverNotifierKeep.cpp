#include "MRIObserverNotifierKeep.h"

void MRI::Observer::ObserverNotifierKeep::Init()
{
	MRI::Observer::ObserverNotifierBase::Init();

	m_endNotifyTime = 0.0F;

	m_notifyFlagTag = MRI::CommonConstant::k_invalidStaticID;
}

void MRI::Observer::ObserverNotifierKeep::DispatchNotify(const float a_elapsedTime)
{
	auto l_observerCache = MRI::Observer::ObserverNotifierBase::GetObserverCache().lock();
	if (!l_observerCache) { return; }

	const bool l_isElapsedStartTime = a_elapsedTime >= MRI::Observer::ObserverNotifierBase::GetBeginNotifyTime();
	const bool l_isElapsedEndTime   = a_elapsedTime >= m_endNotifyTime;

	// 通知開始時間を経過しているかつ通知終了時間を超えていなければ通知開始
	if (l_isElapsedStartTime && !l_isElapsedEndTime)
	{
		l_observerCache->NotifyEvent(MRI::Observer::ObserverNotifierBase::GetNotifyTag() , MRI::StaticID::GetTypeID<MRI::Tag::EventLaneKeep>() , m_notifyFlagTag);
	}
	// 通知時間を経過していれば通知終了
	else if (l_isElapsedEndTime)
	{
		l_observerCache->NotifyEvent(MRI::Observer::ObserverNotifierBase::GetNotifyTag() , MRI::StaticID::GetTypeID<MRI::Tag::EventLaneKeep>() , MRI::TagUtility::InvertBoolFlagTag(m_notifyFlagTag));
	}
}

void MRI::Observer::ObserverNotifierKeep::ResetNotify()
{
	auto l_observerCache = MRI::Observer::ObserverNotifierBase::GetObserverCache().lock();
	if (!l_observerCache) { return; }

	// 通知する予定のフラグと反対のフラグを渡す
	l_observerCache->NotifyEvent(MRI::Observer::ObserverNotifierBase::GetNotifyTag() , MRI::StaticID::GetTypeID<MRI::Tag::EventLaneKeep>() , MRI::TagUtility::InvertBoolFlagTag(m_notifyFlagTag));
}

void MRI::Observer::ObserverNotifierKeep::EditInspector(const float a_notifyTimeLimit)
{
	ImGui::PushID(&m_endNotifyTime);

	ImGui::DragFloat("EndNotifyTime"                               ,
				     &m_endNotifyTime							   ,
					 MRI::EditorCommonConstant::k_defaultDragValue ,
					 m_endNotifyTimeMin							   ,
					 a_notifyTimeLimit);

	ImGui::PopID();
}

void MRI::Observer::ObserverNotifierKeep::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	MRI::Observer::ObserverNotifierBase::Deserialize  (a_json);
	m_notifyFlagTag = MRI::JsonUtility::DeserializeTag(a_json);
	m_endNotifyTime = a_json.value					  ("EndNotifyTime" , 0.0F);
}
nlohmann::json MRI::Observer::ObserverNotifierKeep::Serialize() const
{
	auto l_rootJson = nlohmann::json							    ();
	auto l_baseJson = MRI::Observer::ObserverNotifierBase::Serialize();
	auto l_tagJson  = MRI::JsonUtility::SerializeTag                (m_notifyFlagTag);

	MRI::JsonUtility::UpdateJson(l_rootJson , l_baseJson);
	MRI::JsonUtility::UpdateJson(l_rootJson , l_tagJson);

	l_rootJson["EndNotifyTime"] = m_endNotifyTime;

	return l_rootJson;
}