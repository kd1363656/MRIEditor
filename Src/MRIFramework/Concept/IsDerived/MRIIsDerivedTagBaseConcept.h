#pragma once

namespace MRI::Concept
{
	template <typename Type>
	concept IsDerivedTagBaseConcept = MRI::Concept::IsDerivedBaseConcept<Type , MRI::Tag::TagBase>;
}