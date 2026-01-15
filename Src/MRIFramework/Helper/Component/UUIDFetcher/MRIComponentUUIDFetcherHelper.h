#pragma once

namespace MRI::Helper
{
	template <MRI::Concept::IsDerivedComponentBaseConcept Type>
	class ComponentUUIDFetcherHelper final : public MRI::Helper::ComponentHelperBase
	{
	public:

		ComponentUUIDFetcherHelper ()          = default;
		~ComponentUUIDFetcherHelper() override = default;

		void Init() override
		{
			m_componentCache.reset();

			m_targetGameObjectUUID = GUID_NULL;
		}

		void PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner) override
		{
			if (!a_owner) { return; }

			FindGameObjectComponent();
		}

		void EditPrefabInspector() override
		{
			const auto& l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
			if (!l_sceneCache) { return; }

			ImGui::SetNextItemWidth(MRI::EditorCommonConstant::k_comboWidth);
			ImGui::BeginGroup      ();

			if (const std::string& l_uuidSTR = MRI::UUIDUtility::UUIDToString(m_targetGameObjectUUID);
				!ImGui::BeginCombo("GetComponentGameObjectSelector" , l_uuidSTR.c_str()))
			{
				ImGui::EndGroup();
				return;
			}

			for (const auto& l_gameObject : l_sceneCache->GetGameObjectList())
			{
				if (!l_gameObject) { continue; }

				ImGui::PushID(l_gameObject.get());

				bool l_isSelected = m_targetGameObjectUUID == l_gameObject->GetUUID();

				const std::string& l_selectUUIDSTR = MRI::UUIDUtility::UUIDToString(l_gameObject->GetUUID());

				if (!ImGui::RadioButton(l_selectUUIDSTR.c_str() , l_isSelected))
				{
					ImGui::PopID();
					continue;
				}

				// 選択された項目にカーソルを当てる
				if (l_isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}

				// 選択した"UUID"を格納
				m_targetGameObjectUUID = l_gameObject->GetUUID();
				FindGameObjectComponent                       ();

				ImGui::PopID();
			}

			ImGui::EndCombo();
			ImGui::EndGroup();
		}

		void DeserializeSpawn(const nlohmann::json& a_json) override
		{
			if (a_json.is_null()) { return; }

			m_targetGameObjectUUID = MRI::JsonUtility::DeserializeUUID(a_json);
		}

		nlohmann::json SerializeSpawn() override
		{
			auto l_rootJson = nlohmann::json();

			MRI::JsonUtility::UpdateJson(l_rootJson , MRI::JsonUtility::SerializeUUID(m_targetGameObjectUUID));

			return l_rootJson;
		}

		// 該当する"UUID"を持つゲームオブジェクトを探しそのゲームオブジェクトからコンポーネントを取得する
		void FindGameObjectComponent()
		{
			const auto& l_sceneCache = MRI::SceneManager::GetInstance().GetSceneCache().lock();
			if (!l_sceneCache) { return; }

			// デシリアライズして取得した"UUID"からコンポーネントを取得
			// コンポーネントリストに"TransfprmComponent"だけ存在しないのでif constexpr"で処理分岐
			if constexpr (std::is_same_v<Type , MRI::Component::TransformComponent>)
			{
				for (const auto& l_gameObject : l_sceneCache->GetGameObjectList())
				{
					if (!l_gameObject)                                     { continue; }
					if (l_gameObject->GetUUID() == GUID_NULL)			   { continue; }
					if (l_gameObject->GetUUID() != m_targetGameObjectUUID) { continue; }
					
					// コンオポーネントが格納出来たらこれ以上処理する必要がないので"break"
					m_componentCache = l_gameObject->GetSelfTransformComponentCache();
					break;
				}
			}
			else
			{
				for (const auto& l_gameObject : l_sceneCache->GetGameObjectList())
				{
					if (!l_gameObject)                                     { continue; }
					if (l_gameObject->GetUUID() == GUID_NULL)			   { continue; }
					if (l_gameObject->GetUUID() != m_targetGameObjectUUID) { continue; }
					
					// コンオポーネントが格納出来たらこれ以上処理する必要がないので"break"
					m_componentCache = l_gameObject->template GetComponentCache<Type>();
					break;
				}
			}
		}

		std::weak_ptr<Type> Get() const { return m_componentCache; }

	private:

		std::weak_ptr<Type> m_componentCache;

		UUID m_targetGameObjectUUID = GUID_NULL;
	};
}