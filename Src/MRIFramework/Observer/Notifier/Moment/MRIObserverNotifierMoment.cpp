#include "MRIObserverNotifierMoment.h"

void MRI::Observer::ObserverNotifierMoment::Init()
{
	MRI::Observer::ObserverNotifierBase::Init();

	m_isNotifiedOnce = false;
}

void MRI::Observer::ObserverNotifierMoment::DispatchNotify(const float a_elapsedTime)
{
	auto l_observerCache = MRI::Observer::ObserverNotifierBase::GetObserverCache().lock();
	if (!l_observerCache) { return; }

	// もうすでに通知しているか通知時間が通知開始時間に達していなければ実行しない
	const bool l_isElapsed = a_elapsedTime <= MRI::Observer::ObserverNotifierBase::GetBeginNotifyTime();
	if (l_isElapsed || m_isNotifiedOnce) 
	{
		return;
	}

	if (!l_isElapsed)
	{
		// 通知を行ったことを伝える
		m_isNotifiedOnce = true;

		// "Moment"は毎フレーム通知を"False"にするため
		// "True"意外伝える必要がない
		l_observerCache->NotifyEvent(MRI::Observer::ObserverNotifierBase::GetNotifyTag() , MRI::StaticID::GetTypeID<MRI::Tag::EventLaneMoment>() , MRI::StaticID::GetTypeID<MRI::Tag::BoolTagTrue>());
	}
}

void MRI::Observer::ObserverNotifierMoment::ResetNotify()
{
	// 通知を行ったことを伝える
	m_isNotifiedOnce = false;
	
	MRI::Observer::ObserverNotifierBase::ResetNotify();
}

void MRI::Observer::ObserverNotifierMoment::EditInspector(const float a_notifyTimeLimit)
{
	ImGui::PushID(&m_isNotifiedOnce);

	// フラグが立っているかどうかを文字列で確認
	ImGui::Text ("IsNotifiedOnce : %s" , MRI::EditorUtility::BoolToString(m_isNotifiedOnce));
	ImGui::PopID();
}