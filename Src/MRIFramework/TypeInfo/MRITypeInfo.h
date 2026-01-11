#pragma once

namespace MRI
{
	struct TypeInfo
	{
		const MRI::TypeInfo* const k_baseInfo = nullptr;
		const std::string_view     k_name     = std::string_view();
		const std::uint32_t        k_id       = MRI::CommonConstant::k_invalidStaticID;
	};

	// 関数の明示的特殊化のみを使用
	// ヘッダーで明示的特殊化がなければそもそも使えないので安心して使える
	// 書き方的にテンプレートの特殊化宣言はクラス内部で書けないためクラス外部で書く必要があるため
	// クラスにこの"TypeInfo"を使う関数が欲しければ絶対に関数でこの"TypeInfo"を取得するには"cpp"に書く必要がある
	// しかし安全性が保障されるなら妥協するべきと考えた。
	template <typename Type>
	const MRI::TypeInfo& GetTypeInfo() = delete;

	template <typename Type>
	struct IsTypeInfoRegisterd : std::false_type {};
}

// 親なし版
#define MRI_REGISTER_TYPE_INFO_ROOT(Class)						\
namespace MRI													\
{																\
	template <>													\
	struct MRI::IsTypeInfoRegisterd<Class> : std::true_type {};	\
																\
	template <>													\
	inline const MRI::TypeInfo& GetTypeInfo<Class>()			\
	{															\
		static const MRI::TypeInfo l_info =						\
		{														\
			nullptr							  ,					\
			#Class                            ,					\
			MRI::StaticID::GetTypeID<Class>() 					\
		};														\
		return l_info;											\
	}															\
}

// 親あり版
#define MRI_REGISTER_TYPE_INFO(Class , BaseClass)																											    \
namespace MRI																																				    \
{																																							    \
																																								\
	template <>																																				    \
	struct MRI::IsTypeInfoRegisterd<Class> : std::true_type {};																							        \
																																								\
	static_assert(!std::is_same_v<Class , BaseClass>          , "同一クラスを指定しています。継承関係がはっきりしていません");									\
	static_assert(MRI::IsTypeInfoRegisterd<BaseClass>::value , "BaseClassのTypeInfoが未登録のまま子クラスを登録しようとしています。");						    \
	static_assert(std::derived_from<Class , BaseClass>       , "TypeInfoで定義されている継承関係は間違っています。派生クラスは基底クラスを継承していません。"); \
																																								\
	template <>																																				    \
	inline const MRI::TypeInfo& GetTypeInfo<Class>()																										    \
	{																																						    \
		static const MRI::TypeInfo l_info =																													    \
		{																																					    \
			&GetTypeInfo<BaseClass>()		  ,																													\
			#Class							  ,																												    \
			MRI::StaticID::GetTypeID<Class>() 																												    \
		};																																					    \
		return l_info;																																		    \
	}																																						    \
}