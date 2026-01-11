#pragma once

namespace MRI::Concept
{
	template <typename Type>
	concept IsDerivedInterpolatorModifierBaseConcept = MRI::Concept::IsDerivedBaseConcept<Type , MRI::Modifier::InterpolatorModifierBase>;
}