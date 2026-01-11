#include "MRIObserverNotifierBase.h"

void MRI::Observer::ObserverNotifierBase::Init()
{
	m_notifyTag = MRI::CommonConstant::k_invalidStaticID;

	m_beginNotifyTime = 0.0F;
}

void MRI::Observer::ObserverNotifierBase::ResetNotify()
{
	m_beginNotifyTime = 0.0F;
}

void MRI::Observer::ObserverNotifierBase::Deserialize(const nlohmann::json& a_json)
{
	if (a_json.is_null()) { return; }

	m_notifyTag       = MRI::JsonUtility::DeserializeTag(a_json);
	m_beginNotifyTime = a_json.value                    ("BeginNotifyTime", 0.0F);
}

nlohmann::json MRI::Observer::ObserverNotifierBase::Serialize() const 
{
	auto l_rootJson = nlohmann::json();

	MRI::JsonUtility::SerializeTag(m_notifyTag);

	l_rootJson["BeginNotifyTime"] = m_beginNotifyTime;

	return l_rootJson;
}