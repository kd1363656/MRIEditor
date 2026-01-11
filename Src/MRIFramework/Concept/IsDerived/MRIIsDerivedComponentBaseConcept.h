#pragma once

namespace MRI::Concept
{	
	template <typename Type>
	concept IsDerivedComponentBaseConcept = MRI::Concept::IsDerivedBaseConcept<Type , MRI::Component::ComponentBase>;
}