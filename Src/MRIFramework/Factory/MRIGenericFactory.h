#pragma once

namespace MRI
{
	template <typename Type>
		requires MRI::Concept::IsSmartPTRConcept<Type>
	class GenericFactory final : public MRI::SingletonBase<MRI::GenericFactory<Type>>
	{
	public:
	
		using BaseType = typename Type::element_type;
	
		// "DerivedClass"をファクトリーに登録
		template <typename DerivedType>
			requires MRI::Concept::IsDerivedBaseConcept<DerivedType , BaseType>
		void Register(const std::string& a_typeName)
		{
			// クラス名を取得、ファクトリーラムダ関数を作成
			std::function<Type()> l_factoryMethod;
			
			// もしシェアードポインタ型ならシェアードポインタ型を
			// そうでないかつユニークポインタ型ならユニークポインタ型を、
			// どちらにも該当しないなら"nullptr"を作るファクトリーを保存
			// "C++20"以降の"return"文は右辺値なら"RVO"が強制的に実行されるため所有権のコピーが発生しない
			if constexpr (MRI::TypeTrait::PTRType<Type>::k_kind == MRI::TypeTrait::PTRKind::Shared)
			{
				l_factoryMethod = []() -> Type
				{
					return std::make_shared<DerivedType>();
				};
			}
			else if constexpr (MRI::TypeTrait::PTRType<Type>::k_kind == MRI::TypeTrait::PTRKind::Unique)
			{
				l_factoryMethod = []() -> Type
				{
					return std::make_unique<DerivedType>();
				};
			}
			
			// 登録試行
			m_factoryMap.try_emplace(a_typeName , l_factoryMethod);
		}
	
		Type Create(const std::string& a_className) const
		{
			// マップから登録されているファクトリーメソッドを取得
			auto l_itr = m_factoryMap.find(a_className);

			if (l_itr == m_factoryMap.end())
			{
				return nullptr;
			}
	
			return l_itr->second();
		}
	
		const auto& GetFactoryMap() const { return m_factoryMap; }
	
	private:
	
		std::unordered_map<std::string , std::function<Type()> , MRI::CommonStruct::StringHash , std::equal_to<>> m_factoryMap;
	
		// ==============================
		// "Singleton"
		// ==============================
		friend class MRI::SingletonBase<MRI::GenericFactory<Type>>;
		// "CRTP"で行っているためインスタンス化は派生クラスからのみ行われる
		// 派生クラスでデストラクタを定義しても問題ないように,"virtual"にしておく
		GenericFactory ()	       = default;
		~GenericFactory() override = default;
	};
}

#define MRI_REGISTER_FACTORY_METHOD(FactoryType , DerivedType) \
FactoryType::GetInstance().Register<DerivedType>(#DerivedType)