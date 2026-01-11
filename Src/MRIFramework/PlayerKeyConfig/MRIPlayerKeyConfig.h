#pragma once

// ここでしか使わないコンセプトなのでここで定義してしまう
namespace MRI
{
	template <typename Type>
	concept IsDerivedInputTagBase = std::derived_from<Type , MRI::Tag::InputEventTagBase>;
}

namespace MRI
{
	class PlayerKeyConfig final
	{
	public:

		PlayerKeyConfig () = default;
		~PlayerKeyConfig() = default;

		void Init();
		
		void LoadFile();
		void SaveFile() const;

		void           Deserialize(const nlohmann::json& a_json);
		nlohmann::json Serialize  ()							 const;

		void RegisterInputConfig(const std::uint32_t a_tag , const int a_vkCode);

		template <MRI::IsDerivedInputTagBase Type>
		bool IsInputOnce()
		{
			int l_vkCode = FetchVKCode<Type>();
			if (l_vkCode == NULL)
			{
				return false;
			}

			return MRI::InputManager::GetInstance().IsInputOnce(l_vkCode);
		}

		template <MRI::IsDerivedInputTagBase Type>
		bool IsInputHeld()
		{
			int l_vkCode = FetchVKCode<Type>();
			if (l_vkCode == NULL)
			{
				return false;
			}

			return MRI::InputManager::GetInstance().IsInputHeld(l_vkCode);
		}

		template <MRI::IsDerivedInputTagBase Type>
		int FetchVKCode()
		{
			std::uint32_t l_tag = MRI::StaticID::GetTypeID<Type>();
			return FetchVKCode(l_tag);
		}

		int FetchVKCode(const std::uint32_t a_inputEventTag)
		{
			auto l_itr = m_keyConfigMap.find(a_inputEventTag);
			if (l_itr == m_keyConfigMap.end())
			{
				return NULL;
			}

			return l_itr->second;
		}

		const auto& GetKeyConfigMap    () const { return m_keyConfigMap; }
		auto&       GetWorkKeyConfigMap()       { return m_keyConfigMap; }

	private:

		
		const std::string k_fileIOPath = "Asset/Data/Setting/PlayerKeyConfig/PlayerKeyConfig.json";

		std::unordered_map<std::uint32_t , int> m_keyConfigMap;
	};
}