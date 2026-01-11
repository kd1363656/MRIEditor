#pragma once

namespace MRI::Observer
{
	class ObserverNotifierBase
	{
	public:

		ObserverNotifierBase         () = default;
		virtual ~ObserverNotifierBase() = default;

		virtual void Init();

		virtual void DispatchNotify(const float a_elapsedTime) = 0;

		virtual void ResetNotify();

		template <MRI::Concept::IsDerivedTagBaseConcept BaseType>
		void EditBaseInspector(const float a_notifyTimeLimit)
		{
			ImGui::PushID(&m_notifyTag);

			MRI::EditorUtility::TagRadioButtonSelector<BaseType>();

			ImGui::Separator();

			ImGui::DragFloat("BeginNotifyTime"                     , 
					 &m_beginNotifyTime                            ,
					 MRI::EditorCommonConstant::k_defaultDragValue ,
				     k_notifyTimeMin                               ,
					 a_notifyTimeLimit);

			ImGui::PopID();
		}

		virtual void EditInspector(const float a_notifyTimeLimit) { /*必要に応じてオーバーライドしてください*/ };

		virtual void           Deserialize(const nlohmann::json& a_json);
		virtual nlohmann::json Serialize  ()							 const;

		std::weak_ptr<MRI::Observer::Observer> GetObserverCache() const { return m_observerCache; }

		float GetBeginNotifyTime() const { return m_beginNotifyTime; }
		 
		std::uint32_t GetNotifyTag() const { return m_notifyTag; }

	private:

		static constexpr float k_notifyTimeMin = 0.0F;

		std::weak_ptr<MRI::Observer::Observer> m_observerCache;

		float m_beginNotifyTime = 0.0F;

		std::uint32_t m_notifyTag = MRI::CommonConstant::k_invalidStaticID;
	};
}

MRI_REGISTER_TYPE_INFO_ROOT(MRI::Observer::ObserverNotifierBase);