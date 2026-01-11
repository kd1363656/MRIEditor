#include "MRIObserver.h"

void MRI::Observer::Observer::Init()
{
	m_subscribedEventTagMap.clear();

	m_imGuiSelectedTag = MRI::CommonConstant::k_invalidStaticID;
}

void MRI::Observer::Observer::BeginFrame()
{
	for (auto& [l_key , l_value] : m_subscribedEventTagMap)
	{
		// 瞬間的に記録するイベントは毎フレーム"false"にする
		// そうすることで一フレームしか通知しないイベントを実現できる
		l_value.moment = MRI::StaticID::GetTypeID<MRI::Tag::BoolTagFalse>();
	}
}

void MRI::Observer::Observer::SubscribeEvent(const MRI::Observer::Observer::EventTag a_eventTag)
{
	// 無効なイベントタグを登録しないように"return"
	if (a_eventTag == MRI::CommonConstant::k_invalidStaticID) { return; }

	// イベントに適したキーを格納
	MRI::CommonStruct::EventTypeData l_eventTypeData = {};
	m_subscribedEventTagMap.try_emplace(a_eventTag , l_eventTypeData);
}

void MRI::Observer::Observer::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null())												   { return; }
	if (!MRI::JsonUtility::IsArray(a_json , "SubscribedEventTypeDataMap")) { return; }

	for (const auto& l_json : a_json["SubscribedEventTypeDataMap"])
	{
		const MRI::Observer::Observer::EventTag l_eventTag = MRI::JsonUtility::DeserializeTag(l_json);
		SubscribeEvent																		 (l_eventTag);
	}
}

nlohmann::json MRI::Observer::Observer::Serialize() const
{
	auto l_rootJson  = nlohmann::json       ();
	auto l_jsonArray = nlohmann::json::array();

	// マップ情報を"Json"に保存
	for (const auto& [l_key , l_value] : m_subscribedEventTagMap)
	{
		l_jsonArray.emplace_back(MRI::JsonUtility::SerializeTag(l_key));
	}

	l_rootJson["SubscribedEventTypeDataMap"] = l_jsonArray;

	return l_rootJson;
}

void MRI::Observer::Observer::NotifyEvent(const MRI::Observer::Observer::EventTag a_eventTag , const MRI::Observer::Observer::EventLane a_eventLane , const MRI::Observer::Observer::BoolTag a_setBoolTag)
{
	auto l_itr = m_subscribedEventTagMap.find(a_eventTag);
	if (l_itr == m_subscribedEventTagMap.end()) { return; }

	// 指定されたイベントレーンタグに状態をセット
	if (a_eventLane == MRI::StaticID::GetTypeID<MRI::Tag::EventLaneMoment>())
	{
		l_itr->second.moment = a_setBoolTag;
	}
	else if (a_eventLane == MRI::StaticID::GetTypeID<MRI::Tag::EventLaneKeep>())
	{
		l_itr->second.keep = a_setBoolTag;
	}
}

bool MRI::Observer::Observer::IsEventMatching(const MRI::Observer::Observer::EventTag a_eventTag , const MRI::Observer::Observer::EventLane a_eventLane , const MRI::Observer::Observer::EventTag a_matchCondition) const
{
	MRI::Observer::Observer::BoolTag l_flag = FetchNotification(a_eventTag , a_eventLane);
	return l_flag == a_matchCondition;
}

MRI::CommonStruct::EventTypeData MRI::Observer::Observer::FetchEventTypeData(const MRI::Observer::Observer::EventTag a_receivedEventTag) const
{
	auto l_itr = m_subscribedEventTagMap.find(a_receivedEventTag);
	if (l_itr == m_subscribedEventTagMap.end())
	{
		MRI::CommonStruct::EventTypeData l_eventTypeData = {};
		return l_eventTypeData;
	}

	return l_itr->second;
}

MRI::Observer::Observer::BoolTag MRI::Observer::Observer::FetchNotification(const MRI::Observer::Observer::EventTag a_receivedEventTag , const MRI::Observer::Observer::EventLane a_eventLane) const
{
	auto l_itr = m_subscribedEventTagMap.find(a_receivedEventTag);
	if (l_itr == m_subscribedEventTagMap.end()) 
	{ 
		return MRI::CommonConstant::k_invalidStaticID;
	}

	if (a_eventLane == MRI::StaticID::GetTypeID<MRI::Tag::EventLaneMoment>())
	{
		return l_itr->second.moment;
	}
	else if (a_eventLane == MRI::StaticID::GetTypeID<MRI::Tag::EventLaneKeep>())
	{
		return l_itr->second.keep;
	}

	return MRI::CommonConstant::k_invalidStaticID;
}
