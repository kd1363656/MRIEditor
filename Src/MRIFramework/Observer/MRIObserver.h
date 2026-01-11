#pragma once

namespace MRI::Observer
{
	class Observer final
	{
	private:

		using EventLane = std::uint32_t;
		using EventTag  = std::uint32_t;
		using BoolTag   = std::uint32_t;

	public:

		Observer () = default;
		~Observer() = default;

		void Init();

		void BeginFrame    ();
		void SubscribeEvent(const MRI::Observer::Observer::EventTag a_eventTag);

		template <MRI::Concept::IsDerivedTagBaseConcept BaseType>
		void EditPrefabInspector()
		{
			const auto& l_typeInfoRegistry = MRI::TypeInfoRegistry::GetInstance();

			ImGui::PushID(&m_subscribedEventTagMap);

			// 条件付き生成ファクトリーからタグを選ぶ
			MRI::EditorUtility::TagRadioButtonSelector<BaseType>("TagIDMapSelector" , m_imGuiSelectedTag);

			// 選んだタグを"Map"に追加
			ImGui::SameLine();
			if (ImGui::Button("Add ReceiveEvent"))
			{
				SubscribeEvent(m_imGuiSelectedTag);
			}

			ImGui::Separator();

			auto l_itr = m_subscribedEventTagMap.begin();
			while (l_itr != m_subscribedEventTagMap.end())
			{
				// イテレーターの更新ごとに"ID"を発行
				ImGui::PushID(&l_itr);

				const MRI::TypeInfo* l_typeInfo = l_typeInfoRegistry.FetchTypeInfoByID(l_itr->first);
				if (!l_typeInfo)
				{
					ImGui::PopID();
					l_itr++;
					continue;
				}

				// もしツリーノードが開かれていなければイテレーターを更新(無限ループを起こさないため)
				if (const std::string l_tagName = l_typeInfo->k_name.data();
					!ImGui::TreeNodeEx(l_tagName.c_str() , ImGuiTreeNodeFlags_Framed))
				{
					l_itr++;
					ImGui::PopID();
					continue;
				}

				// 削除されたらイテレーターを更新して"continue"
				ImGui::SameLine();
				if (ImGui::Button("Delete"))
				{
					l_itr = m_subscribedEventTagMap.erase(l_itr);
					ImGui::PopID();
					continue;
				}

				ImGui::Text("MomentEvent : %s" , MRI::EditorUtility::BoolToString(l_itr->second.moment));
				ImGui::Text("KeepEvent   : %s" , MRI::EditorUtility::BoolToString(l_itr->second.keep));
				ImGui::Separator();

				l_itr++;
				ImGui::PopID();
			}

			ImGui::PopID  ();
			ImGui::TreePop();
		}

		void           Deserialize(const nlohmann::json& a_json);
		nlohmann::json Serialize  () const;

		void NotifyEvent(const MRI::Observer::Observer::EventTag a_eventTag , const MRI::Observer::Observer::EventLane a_eventLane , const MRI::Observer::Observer::BoolTag a_setBoolTag);

		bool IsEventMatching(const MRI::Observer::Observer::EventTag a_eventTag , const MRI::Observer::Observer::EventLane a_eventLane , const MRI::Observer::Observer::EventTag a_matchCondition) const;

		MRI::CommonStruct::EventTypeData FetchEventTypeData(const MRI::Observer::Observer::EventTag a_receivedEventTag) const;

		MRI::Observer::Observer::BoolTag FetchNotification(const MRI::Observer::Observer::EventTag a_receivedEventTag , const MRI::Observer::Observer::EventLane a_eventLane) const;

		auto& GetWorkSubscribedEventMap() { return m_subscribedEventTagMap; }

	private:

		std::unordered_map<std::uint32_t , MRI::CommonStruct::EventTypeData> m_subscribedEventTagMap;

		MRI::Observer::Observer::EventTag m_imGuiSelectedTag = MRI::CommonConstant::k_invalidStaticID;
	};
}