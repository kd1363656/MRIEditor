#pragma once

namespace MRI
{
	//=========================================================================
	// 型情報を指定しなくても"TypeInfo"を取得できるレジストリークラス
	//=========================================================================
	class TypeInfoRegistry final : public MRI::SingletonBase<MRI::TypeInfoRegistry>
	{
	public:

		void Register(const MRI::TypeInfo& a_info);

		// 指定した基底クラス、構造体を継承している"TypeInfo"全てを取得
		template <typename Type>
		std::vector<const MRI::TypeInfo*> FetchTypeInfoDerivedFromBaseList() const
		{
			std::vector<const MRI::TypeInfo*> l_list;

			for (const auto& [l_key , l_value] : m_allTypeInfoIDMap)
			{
				if (!l_value) { continue; }

				if (!MRI::TypeInfoUtility::IsDerivedFrom(*l_value , MRI::GetTypeInfo<Type>())) { continue; }
				l_list.emplace_back(l_value);
			}

			return l_list;
		}

		const MRI::TypeInfo* FetchTypeInfoByID  (const std::uint32_t     a_id)   const;
		const MRI::TypeInfo* FetchTypeInfoByName(const std::string_view& a_name) const;

		const auto& GetAllTypeInfoByIDMap() const { return m_allTypeInfoIDMap; }

	private:
	
		// 文字列がキーのマップはデシリアライズする際に使用
		std::unordered_map<std::uint32_t    , const TypeInfo* const>													m_allTypeInfoIDMap;
		std::unordered_map<std::string_view , const TypeInfo* const , MRI::CommonStruct::StringHash , std::equal_to<>> m_allTypeInfoNameMap;

		// ==============================
		// "Singleton"
		// ==============================
		friend class MRI::SingletonBase<MRI::TypeInfoRegistry>;

		TypeInfoRegistry ()          = default;
		~TypeInfoRegistry() override = default;
	};
}