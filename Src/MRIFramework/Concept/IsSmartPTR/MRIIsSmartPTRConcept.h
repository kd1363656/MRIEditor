#pragma once

namespace MRI::Concept
{
	template<typename Type>
	concept IsSharedPTRConcept = MRI::TypeTrait::PTRType<Type>::k_kind == MRI::TypeTrait::PTRKind::Shared;

	template<typename Type>
	concept IsUniquePTRConcept = MRI::TypeTrait::PTRType<Type>::k_kind == MRI::TypeTrait::PTRKind::Unique;

	template <typename Type>
	concept IsSmartPTRConcept = IsSharedPTRConcept<Type> || IsUniquePTRConcept<Type>;
}