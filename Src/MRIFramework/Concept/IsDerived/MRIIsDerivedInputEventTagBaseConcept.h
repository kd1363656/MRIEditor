#pragma once

namespace MRI::Concept
{
	template <typename Type>
	concept IsDerivedInputEventTagBaseConcept = MRI::Concept::IsDerivedBaseConcept<Type , MRI::Tag::InputEventTagBase>;
}