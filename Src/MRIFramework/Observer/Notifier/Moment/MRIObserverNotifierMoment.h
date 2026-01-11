#pragma once

namespace MRI::Observer
{
	class ObserverNotifierMoment final : public MRI::Observer::ObserverNotifierBase
	{
	public:

		ObserverNotifierMoment ()          = default;
		~ObserverNotifierMoment() override = default;

		void Init() override;

		void DispatchNotify(const float a_elapsedTime) override;

		void ResetNotify() override;

		void EditInspector(const float a_notifyTimeLimit) override;

	private:

		bool m_isNotifiedOnce = false;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Observer::ObserverNotifierMoment , MRI::Observer::ObserverNotifierBase);