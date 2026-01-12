#pragma once

namespace MRI::Concept
{
	template <typename Type>
	concept IsDerivedAxisTagBaseConcept = MRI::Concept::IsDerivedBaseConcept<Type , MRI::Tag::AxisTagBase>;
}