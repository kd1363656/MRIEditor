#pragma once

namespace MRI::TypeTrait
{
	enum class PTRKind
	{
		None    ,
		Raw     ,
		Shared  , 
		Unique 
	};
	
	//==============================================================
	// スマートポインタ判定
	//==============================================================
	template <typename Type>
	struct PTRType
	{
		static constexpr MRI::TypeTrait::PTRKind k_kind = MRI::TypeTrait::PTRKind::None;
	};
	
	template <typename Type>
	struct PTRType<Type*>
	{
		static constexpr MRI::TypeTrait::PTRKind k_kind = MRI::TypeTrait::PTRKind::Raw;
	};

	template <typename Type>
	struct PTRType<std::shared_ptr<Type>>
	{
		static constexpr MRI::TypeTrait::PTRKind k_kind = MRI::TypeTrait::PTRKind::Shared;
	};
	
	template <typename Type>
	struct PTRType<std::unique_ptr<Type>>
	{
		static constexpr MRI::TypeTrait::PTRKind k_kind = MRI::TypeTrait::PTRKind::Unique;
	};
}