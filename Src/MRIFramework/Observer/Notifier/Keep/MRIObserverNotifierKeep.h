#pragma once

namespace MRI::Observer
{
	class ObserverNotifierKeep final : public MRI::Observer::ObserverNotifierBase
	{
	public:

		ObserverNotifierKeep ()          = default;
		~ObserverNotifierKeep() override = default;

		void Init() override;

		void DispatchNotify(const float a_elapsedTime) override;

		void ResetNotify() override;

		void EditInspector(const float a_notifyTimeLimit) override;

		void           Deserialize(const nlohmann::json& a_json) override;
		nlohmann::json Serialize  ()							 const override;

	private:

		static constexpr float m_endNotifyTimeMin = 0.0F;

		float m_endNotifyTime = 0.0F;

		std::uint32_t m_notifyFlagTag = MRI::CommonConstant::k_invalidStaticID;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Observer::ObserverNotifierKeep , MRI::Observer::ObserverNotifierBase);