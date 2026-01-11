#pragma once

namespace MRI::Concept
{
	template <typename Type , typename BaseType>
	concept IsDerivedBaseConcept = std::derived_from<Type , BaseType>;
}